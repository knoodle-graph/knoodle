/**************************************************************************/
/* ghi_interface.hpp                                                      */
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

#include <cstdint>

#include "ghi/ghi_definitions.hpp"

namespace kn {
struct GHIDesc {
  uint8_t enable_validation_layers : 1;
  uint8_t enable_sample_anisotropy : 1;
  uint8_t use_shader_float_64 : 1;
};

class IGHI {
 public:
  virtual ~IGHI() = default;

  /**
   * Initialize the GHI.
   * @param[in] desc The description of the GHI.
   * @return True if the GHI was successfully initialized, false otherwise.
   */
  virtual bool initialize(const GHIDesc* desc) = 0;

  /**
   * Shutdown the GHI and release all its resources.
   */
  virtual void shutdown() = 0;
};
}  // namespace kn
