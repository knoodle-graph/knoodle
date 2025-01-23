/**************************************************************************/
/* shader_compiler_structure.cpp                                          */
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

#pragma once

#include <yaml-cpp/yaml.h>
#include <vector>

namespace kn {
struct ShaderCompileHeader {
  const std::string Name;
  const std::filesystem::path Input;
  const std::filesystem::path Output;
  const std::string Entry;
  const std::string Target;
  const std::vector<std::string> Args;
};

struct ShaderPermutationConfig {
  std::string Name;
  std::vector<std::string> Possibilities;
  std::vector<std::string> Defines;
};

struct ShaderConfig {
  std::string Entry;
  std::string Target;
  std::vector<ShaderPermutationConfig> Permutations;
};
}  // namespace kn

namespace YAML {
template <>
struct convert<kn::ShaderPermutationConfig> {
  static Node encode(const kn::ShaderPermutationConfig& rhs) {
    Node node;
    node["name"] = rhs.Name;
    node["possibilities"] = rhs.Possibilities;
    node["defines"] = rhs.Defines;
    return node;
  }

  static bool decode(const Node& node, kn::ShaderPermutationConfig& rhs) {
    if (!node.IsMap() || node.size() != 3) {
      return false;
    }

    rhs.Name = node["name"].as<std::string>();
    rhs.Possibilities = node["possibilities"].as<std::vector<std::string>>();
    rhs.Defines = node["defines"].as<std::vector<std::string>>();

    return true;
  }
};

template <>
struct convert<kn::ShaderConfig> {
  static Node encode(const kn::ShaderConfig& rhs) {
    Node node;
    node["entry"] = rhs.Entry;
    node["target"] = rhs.Target;
    node["permutations"] = rhs.Permutations;
    return node;
  }

  static bool decode(const Node& node, kn::ShaderConfig& rhs) {
    if (!node.IsMap() || node.size() < 2) {
      return false;
    }
    rhs.Entry = node["entry"].as<std::string>();
    rhs.Target = node["target"].as<std::string>();
    rhs.Permutations = node["permutations"].as<std::vector<kn::ShaderPermutationConfig>>();

    return true;
  }
};
}  // namespace YAML
