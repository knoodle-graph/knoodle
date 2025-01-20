/**************************************************************************/
/* ghi.cpp                                                                */
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

#include "ghi/ghi_manager.hpp"
#include "core/config/config_manager.hpp"
#include "core/log/log.hpp"
#include "string_utils.hpp"

#include <array>

#if KN_BUILD_NULL_GHI
#include "ghi/null_ghi/null_ghi.hpp"
#endif

namespace kn {
GHIManager& GHIManager::get_instance() {
  static GHIManager instance;
  return instance;
}

IGHI* GHIManager::create_ghi(const std::string_view& hint /*= ""*/) {
  std::string ghi_name;
  if (!hint.empty()) {
    ghi_name = hint;
  } else {
    auto config = ConfigManager::get_instance().get_value("Graphics.GHI");
    if (config.has_value()) {
      ghi_name = config.value();
    }
  }

  if (_ghi) {
    _ghi->shutdown();
  }

  if (ghi_name.empty()) {
#if KN_BUILD_NULL_GHI

    KN_LOG(LogGHI, Info, "Creating NullGHI");
    _ghi = std::make_unique<NullGHI>();
    return _ghi.get();
#else
    KN_LOG(LogGHI, Error, "No GHI specified in the configuration file.");
    return nullptr;
#endif
  }

  const std::array<std::string, 2> vulkan_names = {"vulkan", "vk"};

  if (std::any_of(vulkan_names.begin(), vulkan_names.end(),
                  [ghi_name](std::string name) { return string_utils::to_lower(ghi_name) == name; })) {
    ghi_name = "vulkan-ghi";
  }

  KN_LOG(LogGHI, Info, "Creating GHI: {}", ghi_name);
  _ghi_library.reset(os::load_library(ghi_name));
  if (!_ghi_library->is_loaded()) {
    KN_LOG(LogGHI, Error, "Failed to load GHI library: {}", ghi_name);
    return nullptr;
  }

  auto is_supported = _ghi_library->get_function<bool (*)()>("is_supported");
  auto create_ghi = _ghi_library->get_function<IGHI* (*)()>("create_ghi");

  if (!is_supported || !create_ghi) {
    KN_LOG(LogGHI, Error, "Invalid GHI library: {}", ghi_name);
    return nullptr;
  }

  if (!is_supported()) {
    KN_LOG(LogGHI, Error, "Unsupported GHI: {}", ghi_name);
    return nullptr;
  }

  _ghi.reset(create_ghi());

  return _ghi.get();
}
}  // namespace kn
