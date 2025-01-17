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

#include "math/kn_math.hpp"

TEST_CASE("Testing kn::math functions") {
  SUBCASE("abs") {
    CHECK(kn::math::abs(-5) == 5);
    CHECK(kn::math::abs(5) == 5);
    CHECK(kn::math::abs(-5.5) == doctest::Approx(5.5));
    CHECK(kn::math::abs(5.5) == doctest::Approx(5.5));
  }

  SUBCASE("min") {
    CHECK(kn::math::min(3, 4) == 3);
    CHECK(kn::math::min(4, 3) == 3);
    CHECK(kn::math::min(3.5, 4.5) == doctest::Approx(3.5));
    CHECK(kn::math::min(4.5, 3.5) == doctest::Approx(3.5));
  }

  SUBCASE("max") {
    CHECK(kn::math::max(3, 4) == 4);
    CHECK(kn::math::max(4, 3) == 4);
    CHECK(kn::math::max(3.5, 4.5) == doctest::Approx(4.5));
    CHECK(kn::math::max(4.5, 3.5) == doctest::Approx(4.5));
  }

  SUBCASE("clamp") {
    CHECK(kn::math::clamp(5, 1, 10) == 5);
    CHECK(kn::math::clamp(0, 1, 10) == 1);
    CHECK(kn::math::clamp(15, 1, 10) == 10);
  }

  SUBCASE("pow2") {
    CHECK(kn::math::pow2(3) == 9);
    CHECK(kn::math::pow2(3.0) == doctest::Approx(9.0));
  }

  SUBCASE("pow3") {
    CHECK(kn::math::pow3(2) == 8);
    CHECK(kn::math::pow3(2.0) == doctest::Approx(8.0));
  }

  SUBCASE("pow4") {
    CHECK(kn::math::pow4(2) == 16);
    CHECK(kn::math::pow4(2.0) == doctest::Approx(16.0));
  }

  SUBCASE("pow5") {
    CHECK(kn::math::pow5(2) == 32);
    CHECK(kn::math::pow5(2.0) == doctest::Approx(32.0));
  }

  SUBCASE("sqrt") {
    CHECK(kn::math::sqrt(4) == 2);
    CHECK(kn::math::sqrt(4.0) == doctest::Approx(2.0));
  }

  SUBCASE("rsqrt") {
    CHECK(kn::math::rsqrt(4.0) == doctest::Approx(0.5).epsilon(0.01));
  }

  SUBCASE("lerp") {
    CHECK(kn::math::lerp(0.0, 10.0, 0.5) == doctest::Approx(5.0));
  }
}
