/**************************************************************************/
/* os.hpp                                                                 */
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
#include "core/common.hpp"

namespace kn {
class os {
 public:
  /**
   * Represents a dynamic library.
   */
  class Library {
    friend class os;

    explicit Library(void* handle, std::filesystem::path path) : _handle(handle), _path(std::move(path)) {}

   public:
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    ~Library() {
      if (is_loaded()) {
        os::free_library(_handle);
      }
    }

    template <typename T>
    [[nodiscard]] inline T get_function(const char* name) const {
      return reinterpret_cast<T>(get_function_impl(_handle, name));
    }

    /** Returns the native handle of the library. */
    [[nodiscard]] inline void* get_os_handle() const { return _handle; }

    /** Returns the path of the library. */
    [[nodiscard]] inline std::filesystem::path get_path() const { return _path; }

    /** Checks if the library is loaded. */
    [[nodiscard]] inline bool is_loaded() const { return _handle != nullptr; }

    explicit operator bool() const { return is_loaded(); }

   private:
    void* _handle;
    std::filesystem::path _path;
  };

  /**
   * Loads a dynamic library.
   * @param[in] path The path to the dynamic library.
   * @return pointer to the loaded library.
   */
  [[nodiscard]] inline static Library* load_library(std::filesystem::path path) {
    return load_library_impl(path);
  }

  /**
   * Frees a dynamic library.
   * @param[in] library The library to free.
   */
  inline static bool free_library(void* library) { return free_library_impl(library); }

 private:
  static KN_API Library* load_library_impl(std::filesystem::path path);
  static KN_API bool free_library_impl(void* library);
  static KN_API void* get_function_impl(void* library, const char* name);
};
}  // namespace kn
