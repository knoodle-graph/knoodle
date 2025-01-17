/**************************************************************************/
/* kn_assert.hpp                                                          */
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

#include <cassert>
#include <iostream>

#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() (__nop(), __debugbreak())
#else
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#define ensure(condition)                                             \
  ((condition) || ([](const char* cond, const char* file, int line) { \
     std::cerr << "Ensure failed: (" << cond << "), "                 \
               << "file " << file << ", line " << line << '\n';       \
     DEBUG_BREAK();                                                   \
     return false;                                                    \
   }(#condition, __FILE__, __LINE__)))

#define ensure_msg(condition, msg)                                                         \
  ((condition) || ([](const char* cond, const char* file, int line, const char* message) { \
     std::cerr << "Ensure failed: (" << cond << "), "                                      \
               << "file " << file << ", line " << line << '\n'                             \
               << "Message: " << message << '\n';                                          \
     DEBUG_BREAK();                                                                        \
     return false;                                                                         \
   }(#condition, __FILE__, __LINE__, msg)))
