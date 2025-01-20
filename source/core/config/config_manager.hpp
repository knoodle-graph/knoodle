/**************************************************************************/
/* config_manager.hpp                                                     */
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

#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

#include "core/common.hpp"

namespace kn {
/**
 * Manages the configuration of the application.
 * The configuration is read from a file and stored in memory.
 */
class KN_API ConfigManager {
  ConfigManager() = default;

 public:
  ~ConfigManager() = default;

  static ConfigManager& get_instance();

  /**
   * Loads the configuration from a file and append it to the current configuration.
   * @param file_path The path to the file to load.
   * @return True if the file was successfully loaded, false otherwise.
   */
  bool load_config(const std::filesystem::path& file_path);

  /** Return a set of all the registered sections. */
  [[nodiscard]] std::set<std::string> get_sections() const;

  /**
   * Returns a value by key.
   * @param key The key to search for.
   * @return The value associated with the key, if it exists.
   */
  [[nodiscard]] std::optional<std::string> get_value(const std::string_view& key) const;

  /**
   * Returns a value by key as integer.
   * @param key The key to search for.
   * @return The value associated with the key, if it exists.
   */
  [[nodiscard]] std::optional<int32_t> get_int_value(const std::string_view& key) const;

  /**
   * Returns a value by key as real_t.
   * @param key The key to search for.
   * @return The value associated with the key, if it exists.
   */
  [[nodiscard]] std::optional<real_t> get_real_value(const std::string_view& key) const;

  /**
   * Returns a value by key as boolean.
   * @param key The key to search for.
   * @return The value associated with the key, if it exists.
   */
  [[nodiscard]] std::optional<bool> get_bool_value(const std::string_view& key) const;

 protected:
  /**
   * Parses a line from the configuration file.
   * @param line The line to parse.
   */
  virtual void parse_line(const std::string_view& line);
  /**
   * Parses a key-value pair from a line.
   * @param line The line to parse.
   */
  virtual void parse_key_value(const std::string_view& line);
  /**
   * Parses a comment from a line.
   * @param line The line to parse.
   */
  virtual void parse_comment(const std::string_view& line);
  /**
   * Parses a section from a line.
   * @param line The line to parse.
   */
  virtual void parse_section(const std::string_view& line);
  /**
   * Parses a blank line from a line.
   * @param line The line to parse.
   */
  virtual void parse_blank(const std::string_view& line);
  /**
   * Parses a line from the configuration file.
   * @param line The line to parse.
   */

 private:
  std::unordered_map<std::string, std::string> _config;

  std::string _current_section;
};
}  // namespace kn
