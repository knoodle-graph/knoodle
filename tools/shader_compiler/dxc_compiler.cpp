/**************************************************************************/
/* dxc_compiler.cpp                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                                Knoodle                                 */
/*                        https://knoodlegraph.org                        */
/**************************************************************************/
/* Copyright (c) 2025 Knoodle contributors (vide AUTHORS.md)              */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include <windows.h>

#include <dxc/dxcapi.h>
#include <wrl.h>
#include <cstdint>
#include <cwchar>
#include <filesystem>
#include <iostream>
#include <set>
#include "log/log.hpp"
#include "shader_compiler_structures.hpp"

using namespace Microsoft::WRL;

namespace kn {
std::wstring convertToWideString(const char* str) {
  std::wstring wstr;

  std::mbstate_t state = std::mbstate_t();
  std::size_t len = 1 + std::mbsrtowcs(nullptr, &str, 0, &state);
  wstr.resize(len);
  std::mbsrtowcs(&wstr[0], &str, wstr.size(), &state);
  return wstr;
}

bool compile(const ShaderCompileHeader& shaderHeader) {
  ComPtr<IDxcUtils> pUtils;
  ComPtr<IDxcCompiler3> pCompiler;

  DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
  DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

  ComPtr<IDxcIncludeHandler> pIncludeHandler;
  pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

  std::wstring Filename = shaderHeader.Input.wstring();
  std::wstring OutputFilename = std::filesystem::path(shaderHeader.Output).replace_extension("spv").wstring();

  std::vector<LPCWSTR> Args = {
      Filename.c_str(),
      L"-spirv",
  };

  std::wstring WideEntry;
  if (!shaderHeader.Entry.empty()) {
    Args.push_back(L"-E");
    WideEntry = convertToWideString(shaderHeader.Entry.c_str());
    Args.push_back(WideEntry.c_str());
  }

  std::wstring WideTarget = convertToWideString(shaderHeader.Target.c_str());
  if (!shaderHeader.Target.empty()) {
    Args.push_back(L"-T");
    WideTarget = convertToWideString(shaderHeader.Target.c_str());
    Args.push_back(WideTarget.c_str());
  }

  Args.push_back(L"-Fo");
  Args.push_back(OutputFilename.c_str());

  std::vector<std::wstring> WideArgs;
  for (const auto& ShaderArg : shaderHeader.Args) {
    WideArgs.push_back(convertToWideString(ShaderArg.c_str()));
  }

  for (const auto& WideArg : WideArgs) {
    Args.push_back(L"-D");
    Args.push_back(WideArg.c_str());
  }

  ComPtr<IDxcBlobEncoding> pSource = nullptr;
  pUtils->LoadFile(Filename.c_str(), nullptr, &pSource);
  DxcBuffer Source;

  if (pSource->GetBufferSize()) {
    Source.Ptr = pSource->GetBufferPointer();
    Source.Size = pSource->GetBufferSize();
    Source.Encoding = DXC_CP_ACP;
  }

  ComPtr<IDxcResult> pResults;
  pCompiler->Compile(&Source,                 // Source buffer.
                     Args.data(),             // Array of pointers to arguments.
                     Args.size(),             // Number of arguments.
                     pIncludeHandler.Get(),   // User-provided interface to handle #include directives (optional).
                     IID_PPV_ARGS(&pResults)  // Compiler output status, buffer, and errors.
  );

  ComPtr<IDxcBlobUtf8> pErrors = nullptr;
  pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);

  if (pErrors != nullptr && pErrors->GetStringLength() != 0) {
    KN_LOG(LogDxc, Error, "{}", pErrors->GetStringPointer());
    return false;
  }

  HRESULT hrStatus;
  pResults->GetStatus(&hrStatus);
  if (FAILED(hrStatus)) {
    KN_LOG(LogDxc, Error, "Compilation Failed");
    return false;
  }

  ComPtr<IDxcBlob> pShader = nullptr;
  ComPtr<IDxcBlobUtf16> pShaderName = nullptr;
  pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShader), &pShaderName);
  if (pShader != nullptr) {
    FILE* fp = NULL;
    _wfopen_s(&fp, pShaderName->GetStringPointer(), L"wb");
    if (fp) {
      fwrite(pShader->GetBufferPointer(), pShader->GetBufferSize(), 1, fp);
      fclose(fp);
    }
  }

  return true;
}
}  // namespace kn
