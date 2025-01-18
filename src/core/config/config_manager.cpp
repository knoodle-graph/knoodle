/**************************************************************************/
/* config_manager.cpp                                                     */
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

#include "config/config_manager.hpp"

#include <algorithm>
#include <array>
#include <fstream>

#include "string_utils.hpp"

namespace kn {
ConfigManager& ConfigManager::get_instance() {
  static ConfigManager instance;
  return instance;
}

bool ConfigManager::load_config(const std::filesystem::path& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    return false;
  }
  std::string line;
  while (std::getline(file, line)) {
    line = string_utils::trim(line);
    parse_line(line);
  }
  return true;
}

std::set<std::string> ConfigManager::get_sections() const {
  std::set<std::string> sections;
  for (const auto& [key, _] : _config) {
    const auto separator = key.find('.');
    if (separator != std::string::npos) {
      sections.insert(key.substr(0, separator));
    }
  }
  return sections;
}

std::optional<std::string> ConfigManager::get_value(const std::string_view& key) const {
  const auto it = _config.find(std::string(key));
  if (it == _config.end()) {
    return std::nullopt;
  }
  return it->second;
}

std::optional<int32_t> ConfigManager::get_int_value(const std::string_view& key) const {
  const auto value = get_value(key);
  if (!value.has_value()) {
    return std::nullopt;
  }
  return std::stoi(value.value());
}

std::optional<real_t> ConfigManager::get_real_value(const std::string_view& key) const {
  const auto value = get_value(key);
  if (!value.has_value()) {
    return std::nullopt;
  }
  return static_cast<real_t>(std::stof(value.value()));
}

std::optional<bool> ConfigManager::get_bool_value(const std::string_view& key) const {
  const auto value = get_value(key);
  if (!value.has_value()) {
    return std::nullopt;
  }

  constexpr std::array<std::string_view, 4> true_values = {"true", "yes", "on", "1"};

  return std::any_of(true_values.cbegin(), true_values.cend(),
                     [value](std::string_view v) { return v == string_utils::to_lower(*value); });
}

void ConfigManager::parse_line(const std::string_view& line) {
  if (line.empty()) {
    parse_blank(line);
    return;
  }
  if (line[0] == '#') {
    parse_comment(line);
    return;
  }
  if (line[0] == '[') {
    parse_section(line);
    return;
  }
  parse_key_value(line);
}

void ConfigManager::parse_blank(const std::string_view&) {
  // Do nothing.
}

void ConfigManager::parse_comment(const std::string_view&) {
  // Do nothing.
}

void ConfigManager::parse_section(const std::string_view& line) {
  _current_section = line.substr(1, line.size() - 2);
}

void ConfigManager::parse_key_value(const std::string_view& line) {
  const auto separator = line.find('=');
  if (separator == std::string::npos) {
    return;
  }
  auto key = std::string(line.substr(0, separator));
  if (!_current_section.empty())
    key = _current_section + '.' + key;
  const auto value = line.substr(separator + 1);
  _config.emplace(key, std::string(value));
}

}  // namespace kn
