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
#include <cerrno>
#include <cstdint>
#include <iostream>

using namespace Microsoft::WRL;

namespace kn {
bool compile() {
  ComPtr<IDxcUtils> pUtils;
  ComPtr<IDxcCompiler3> pCompiler;

  DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
  DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

  ComPtr<IDxcIncludeHandler> pIncludeHandler;
  pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

  LPCWSTR pszArgs[] = {L"D:/Dev/myshader.hlsl", L"-spirv", L"-E", L"main", L"-T", L"ps_6_0", L"-Fo",
                       L"D:/Dev/myshader.spv"};

  ComPtr<IDxcBlobEncoding> pSource = nullptr;
  pUtils->LoadFile(L"D:/Dev/myshader.hlsl", nullptr, &pSource);
  DxcBuffer Source;
  Source.Ptr = pSource->GetBufferPointer();
  Source.Size = pSource->GetBufferSize();
  Source.Encoding = DXC_CP_ACP;

  ComPtr<IDxcResult> pResults;
  pCompiler->Compile(&Source,                 // Source buffer.
                     pszArgs,                 // Array of pointers to arguments.
                     _countof(pszArgs),       // Number of arguments.
                     pIncludeHandler.Get(),   // User-provided interface to handle #include directives (optional).
                     IID_PPV_ARGS(&pResults)  // Compiler output status, buffer, and errors.
  );

  ComPtr<IDxcBlobUtf8> pErrors = nullptr;
  pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);

  if (pErrors != nullptr && pErrors->GetStringLength() != 0) {
    std::cerr << pErrors->GetStringPointer() << std::endl;
    return false;
  }

  HRESULT hrStatus;
  pResults->GetStatus(&hrStatus);
  if (FAILED(hrStatus)) {
    wprintf(L"Compilation Failed\n");
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
