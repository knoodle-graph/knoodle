/**************************************************************************/
/* test_math_ops.cpp                                                      */
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "config/config_manager.hpp"

TEST_CASE("ConfigManager") {
  SUBCASE("load_config") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();
    CHECK(config_manager.load_config("D:/Dev/knoodle/config/engine.ini"));
  }

  SUBCASE("get_sections") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();

    auto sections = config_manager.get_sections();
    CHECK(sections.size() != 0);
    CHECK(sections.find("General") != sections.end());
    CHECK(sections.find("Window") != sections.end());
    CHECK(sections.find("Audio") != sections.end());
    CHECK(sections.find("Graphics") != sections.end());
  }

  SUBCASE("get_value") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();

    auto value = config_manager.get_value("General.AppName");
    CHECK(value.has_value());

    value = config_manager.get_value("General.Dummy");
    CHECK(!value.has_value());
  }

  SUBCASE("get_int_value") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();
    auto value = config_manager.get_int_value("Window.Width");
    CHECK(value.has_value());
    CHECK(value.value() == 1280);
  }

  SUBCASE("get_real_value") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();
    auto value = config_manager.get_real_value("Window.AspectRatio");
    CHECK(value.has_value());
    CHECK(value.value() == doctest::Approx(1.778));
  }

  SUBCASE("get_bool_value") {
    kn::ConfigManager& config_manager = kn::ConfigManager::get_instance();
    auto value = config_manager.get_bool_value("Window.Fullscreen");
    CHECK(value.has_value());
    CHECK(!value.value());
  }
}
