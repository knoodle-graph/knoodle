/**************************************************************************/
/* ghi_manager.hpp                                                        */
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

#include <memory>
#include <string_view>
#include "ghi_api.hpp"
#include "ghi_interface.hpp"
#include "os/os.hpp"

namespace kn {

/**
 * Managers the graphics hardware interface.
 */
class GHIManager {
  GHIManager() = default;

 public:
  GHIManager(const GHIManager&) = delete;
  GHIManager& operator=(const GHIManager&) = delete;

  ~GHIManager() = default;

  KN_GHI_API IGHI* create_ghi(const std::string_view& hint = "");

  inline IGHI* get_ghi() const { return _ghi.get(); }

  /**
   * Get the GHI manager instance.
   * @return The GHI manager instance.
   */
  KN_GHI_API static GHIManager& get_instance();

 private:
  std::unique_ptr<IGHI> _ghi;
  std::unique_ptr<os::Library> _ghi_library;
};
}  // namespace kn
