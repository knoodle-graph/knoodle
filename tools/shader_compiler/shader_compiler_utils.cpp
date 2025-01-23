/**************************************************************************/
/* shader_compiler_utils.cpp                                              */
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

#include <yaml-cpp/yaml.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include "log/log.hpp"
#include "shader_compiler_structures.hpp"
#include "string_utils.hpp"

namespace kn {
bool processYmlFile(const std::filesystem::path& filename, ShaderConfig& shaderConfig) {
  try {
    YAML::Node Config = YAML::LoadFile(filename.string());
    if (Config.IsNull()) {
      KN_LOG(LogShaderCompiler, Error, "Invalid YAML file: {}", filename.filename().string());
      return false;
    }

    shaderConfig = Config.as<ShaderConfig>();
    KN_LOG(LogShaderCompiler, Info, "Processed YAML file: {}", filename.string());

  } catch (const std::exception& e) {
    KN_LOG(LogShaderCompiler, Error, "Failed to process yml file: {} -- Exception caught: {}", filename.string(),
           e.what());
    return false;
  }

  return true;
}

bool setupPermutations(const ShaderConfig& shaderConfig, std::set<std::filesystem::path>& processedFiles) {
  for (const auto& permutation : shaderConfig.Permutations) {
	  const std::filesystem::path outputDir = *processedFiles.begin();
  }

  return true;
}

bool preProcessShader(const std::filesystem::path& inputFile, std::set<std::filesystem::path>& processedFiles) {
  KN_LOG(LogShaderCompiler, Info, "Starting preprocessing for shader: {}", inputFile.string());

  if (!std::filesystem::exists(inputFile) || !std::filesystem::is_regular_file(inputFile)) {
    KN_LOG(LogShaderCompiler, Error, "Invalid input file: {}", inputFile.string());
    return false;
  }

  std::ifstream File(inputFile);
  if (!File.is_open() || !File.good()) {
    KN_LOG(LogShaderCompiler, Error, "Failed to open file: {}", inputFile.string());
    return false;
  }

  const std::filesystem::path OutputDir = std::filesystem::temp_directory_path() / "knoodle" / "shader_compiler";
  if (!std::filesystem::exists(OutputDir)) {
    std::error_code ec;
    if (!std::filesystem::create_directories(OutputDir, ec)) {
      KN_LOG(LogShaderCompiler, Error, "Failed to create directory: {} -- {}", OutputDir.string(), ec.message());
      return false;
    }
    KN_LOG(LogShaderCompiler, Info, "Created directory: {}", OutputDir.string());
  }

  std::filesystem::path YmlFile = inputFile;
  const bool HasYamlFile = std::filesystem::exists(YmlFile.replace_extension("yaml")) ||
                           std::filesystem::exists(YmlFile.replace_extension("yml"));

  if (HasYamlFile) {
    std::error_code ec;
    std::filesystem::copy_options options = std::filesystem::copy_options::overwrite_existing;
    if (!std::filesystem::copy_file(YmlFile, OutputDir / YmlFile.filename(), options, ec)) {
      KN_LOG(LogShaderCompiler, Error, "Failed to copy file: {} -- {}", YmlFile.string(), ec.message());
      return false;
    }
    KN_LOG(LogShaderCompiler, Info, "Copied YAML file: {}", YmlFile.string());
  }

  std::string Line;
  int32_t LineNumber = 0;

  std::stringstream YmlContent;
  std::stringstream HlslContent;

  try {
    while (std::getline(File, Line)) {
      LineNumber++;

      if (Line.empty()) {
        continue;
      }

      // Enter Yml configuration block
      if (Line == "---") {
        if (HasYamlFile) {
          KN_LOG(LogShaderCompiler, Error, "YAML file already exists at {}", YmlFile.string());
          return false;
        }

        bool IsInsideYml = true;

        while (std::getline(File, Line)) {
          if (Line == "...") {
            IsInsideYml = false;
            break;
          }

          YmlContent << Line << std::endl;
        }

        if (IsInsideYml) {
          KN_LOG(LogShaderCompiler, Error, "YAML block not closed at {}:{}", inputFile.string(), LineNumber);
          return false;
        }
        KN_LOG(LogShaderCompiler, Info, "Processed YAML block in file: {}", inputFile.string());
      } else {  // Enter HLSL code block
        HlslContent << Line << std::endl;
      }
    }

    if (!YmlContent.str().empty()) {
      std::ofstream Yml(OutputDir / YmlFile.filename());
      if (!Yml.is_open() || !Yml.good()) {
        KN_LOG(LogShaderCompiler, Error, "Failed to open file: {}", YmlFile.string());
        return false;
      }

      Yml << YmlContent.rdbuf();
      Yml.close();
      KN_LOG(LogShaderCompiler, Info, "Written YAML content to file: {}", (OutputDir / YmlFile.filename()).string());
    }

    if (!HlslContent.str().empty()) {
      std::filesystem::path HlslFile = OutputDir / inputFile.filename().replace_extension("hlsl");
      std::ofstream Hlsl(HlslFile);
      if (!Hlsl.is_open() || !Hlsl.good()) {
        KN_LOG(LogShaderCompiler, Error, "Failed to open file: {}", HlslFile.string());
        return false;
      }

      Hlsl << HlslContent.rdbuf();
      Hlsl.close();
      KN_LOG(LogShaderCompiler, Info, "Written HLSL content to file: {}", HlslFile.string());

      processedFiles.clear();
      processedFiles.emplace(std::move(HlslFile));
    }
  } catch (const std::exception& e) {
    KN_LOG(LogShaderCompiler, Error, "Exception caught: {}", e.what());
    return false;
  }

  ShaderConfig ShaderConfig;
  if (!processYmlFile(OutputDir / YmlFile.filename(), ShaderConfig)) {
    return false;
  }

  if (!setupPermutations(ShaderConfig, processedFiles)) {
    return false;
  }

  KN_LOG(LogShaderCompiler, Info, "Preprocessing completed for shader: {}", inputFile.string());
  return true;
}
}  // namespace kn
