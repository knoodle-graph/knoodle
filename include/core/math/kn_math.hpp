/**************************************************************************/
/* vector3.hpp                                                            */
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

namespace kn::math {
using real_t = float;

template <typename T>
concept number = std::integral<T> or std::floating_point<T>;

/** @brief Returns the absolute value of x.
 * @param x The value to return the absolute value of.
 * @return The absolute value of x.
 */
constexpr auto abs(number auto x) {
  return x < 0 ? -x : x;
}

/** @brief Returns the minimum of a and b.
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @return The minimum of a and b.
 */
constexpr auto min(number auto a, number auto b) {
  return a < b ? a : b;
}

/** @brief Returns the maximum of a and b.
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @return The maximum of a and b.
 */
constexpr auto max(number auto a, number auto b) {
  return a > b ? a : b;
}

/** @brief Returns the clamp of x between min and max.
 * @param x The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value of x.
 */
constexpr auto clamp(number auto x, number auto min, number auto max) {
  return x < min ? min : (x > max ? max : x);
}

/** @brief Returns the square of x.
 * @param x The value to square.
 * @return The square of x.
 */
constexpr auto pow2(number auto x) {
  return x * x;
}

/** @brief Returns the cube of x.
 * @param x The value to cube.
 * @return The cube of x.
 */
constexpr auto pow3(number auto x) {
  return x * x * x;
}

/** @brief Returns the value of x to the power of 4.
 * @param x The value to raise to the power of 4.
 * @return The value of x to the power of 4.
 */
constexpr auto pow4(number auto x) {
  return x * x * x * x;
}

/** @brief Returns the value of x to the power of 5.
 * @param x The value to raise to the power of 5.
 * @return The value of x to the power of 5.
 */
constexpr auto pow5(number auto x) {
  return x * x * x * x * x;
}

/** @brief Returns square root of x.
 * @param x The value to return the square root of.
 * @return The square root of x.
 */
auto sqrt(number auto x) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::sqrtf(x);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::sqrt(x);
  else
    return sqrt(static_cast<real_t>(x));
}

/** @brief Returns fast inverse square root of x.
 * Implements the Quake III algorithm.
 * @param x The value to return the inverse square root of.
 * @return The inverse square root of x.
 */
constexpr auto rsqrt(number auto x) {
  assert(x > 0);

  long i;
  float x2, y;
  const float threehalfs = 1.5f;
  const float number = static_cast<float>(x);

  x2 = number * 0.5f;
  y = number;
  i = *(long*)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float*)&i;
  y = y * (threehalfs - (x2 * y * y));
  y = y * (threehalfs - (x2 * y * y));

  return y;
}

/** @brief Returns the linear interpolation between a and b at t.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The linear interpolation between a and b at t.
 */
constexpr auto lerp(number auto a, number auto b, number auto t) {
  return a + t * (b - a);
}

/** @brief Returns the smoothstep interpolation between a and b at t.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The smoothstep interpolation between a and b at t.
 */
constexpr auto smoothstep(number auto a, number auto b, number auto t) {
  t = clamp((t - a) / (b - a), 0.0, 1.0);
  return t * t * (3 - 2 * t);
}

/** @brief Floor function.
 * @param x The value to floor.
 * @return The floor of x.
 */
constexpr auto floor(number auto x) {
  if constexpr (std::integral<decltype(x)>)
    return x;
  else
    return static_cast<decltype(x)>(x);
}

/** @brief Floor function.
 * @param x The value to floor.
 * @return The floor of x.
 */
template <number T, number U>
constexpr T floor_to(U x) {
  T result = static_cast<T>(x);
  result -= (static_cast<U>(result) > x);
  return result;
}

/** @brief Ceil function.
 * @param x The value to ceil.
 * @return The ceil of x.
 */
constexpr auto ceil(number auto x) {
  if constexpr (std::integral<decltype(x)>)
    return x;
  else
    return static_cast<decltype(x)>(x) + 1;
}

/** @brief Ceil function.
 * @param x The value to ceil.
 * @return The ceil of x.
 */
template <number T, number U>
constexpr T ceil_to(U x) {
  T result = static_cast<T>(x);
  result += (static_cast<U>(result) < x);
  return result;
}

/** @brief Round function.
 * @param x The value to round.
 * @return The round of x.
 */
constexpr auto round(number auto x) {
  return floor(x + 0.5);
}

/** @brief Round function.
 * @param x The value to round.
 * @return The round of x.
 */
template <number T, number U>
constexpr T round_to(U x) {
  return floor_to<T>(x + 0.5);
}

/** @brief Returns fractional part of x.
 * @param x The value to return the fractional part of.
 * @return The fractional part of x.
 */
constexpr auto fractional(std::floating_point auto x) {
  return x - ::trunc(x);
}

/** @brief Returns fractional part of x.
 * @param x The value to return the fractional part of.
 * @return The fractional part of x.
 */
constexpr auto frac(std::floating_point auto x) {
  return x - floor_to<decltype(x)>(x);
}

/** @brief Returns the sign of x.
 * @param x The value to return the sign of.
 * @return The sign of x.
 */
constexpr auto sign(number auto x) {
  return x < 0 ? -1 : 1;
}

/** @brief Returns the modulo of x and y.
 * @param x The dividend.
 * @param y The divisor.
 * @return The modulo of x and y.
 */
constexpr auto modf(const std::floating_point auto x, std::floating_point auto* y) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::modff(x, y);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::modf(x, y);
  else
    return modf(static_cast<real_t>(x), y);
}

/** @brief Returns the exponential function of x.
 * @param x The value to return the exponential function of.
 * @return The exponential function of x.
 */
constexpr auto exp(number auto x) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::expf(x);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::exp(x);
  else
    return exp(static_cast<real_t>(x));
}

/** @brief Returns the exp2 function of x.
 * @param x The value to return the exp2 function of.
 * @return The exp2 function of x.
 */
constexpr auto exp2(number auto x) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::powf(2.0f, x);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::pow(2.0, x);
  else
    return exp2(static_cast<real_t>(x));
}

/** @brief Returns the natural logarithm of x.
 * @param x The value to return the natural logarithm of.
 * @return The natural logarithm of x.
 */
constexpr auto log(number auto x) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::logf(x);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::log(x);
  else
    return log(static_cast<real_t>(x));
}

/** @brief Returns the base 2 logarithm of x.
 * @param x The value to return the base 2 logarithm of.
 * @return The base 2 logarithm of x.
 */
constexpr auto log2(number auto x) {
  if constexpr (std::same_as<decltype(x), float>)
    return ::log2f(x);
  else if constexpr (std::same_as<decltype(x), double>)
    return ::log2(x);
  else
    return log2(static_cast<real_t>(x));
}

}  // namespace kn::math
