#pragma once

#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>

#define TIMERSTART(label)                                                \
  std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
  a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                            \
  b##label = std::chrono::system_clock::now();                      \
  std::chrono::duration<double> delta##label = b##label - a##label; \
  std::cout << std::fixed << "# elapsed time (" << #label << "): "                \
            << delta##label.count() << "s" << std::endl;

// safe division
#define SDIV(x, y) (((x) + (y) - 1) / (y))

// no_init_t
#include <type_traits>

template <class T>
class no_init_t
{
public:
  static_assert(std::is_fundamental<T>::value &&
                    std::is_arithmetic<T>::value,
                "wrapped type must be a fundamental, numeric type");

  // do nothing
  constexpr no_init_t() noexcept {}

  // convertible from a T
  constexpr no_init_t(T value) noexcept : v_(value) {}

  // act as a T in all conversion contexts
  constexpr operator T() const noexcept { return v_; }

  // negation on value and bit level
  constexpr no_init_t &operator-() noexcept
  {
    v_ = -v_;
    return *this;
  }
  constexpr no_init_t &operator~() noexcept
  {
    v_ = ~v_;
    return *this;
  }

  // prefix increment/decrement operators
  constexpr no_init_t &operator++() noexcept
  {
    v_++;
    return *this;
  }
  constexpr no_init_t &operator--() noexcept
  {
    v_--;
    return *this;
  }

  // postfix increment/decrement operators
  constexpr no_init_t operator++(int) noexcept
  {
    auto old(*this);
    v_++;
    return old;
  }
  constexpr no_init_t operator--(int) noexcept
  {
    auto old(*this);
    v_--;
    return old;
  }

  // assignment operators
  constexpr no_init_t &operator+=(T v) noexcept
  {
    v_ += v;
    return *this;
  }
  constexpr no_init_t &operator-=(T v) noexcept
  {
    v_ -= v;
    return *this;
  }
  constexpr no_init_t &operator*=(T v) noexcept
  {
    v_ *= v;
    return *this;
  }
  constexpr no_init_t &operator/=(T v) noexcept
  {
    v_ /= v;
    return *this;
  }

  // bit-wise operators
  constexpr no_init_t &operator&=(T v) noexcept
  {
    v_ &= v;
    return *this;
  }
  constexpr no_init_t &operator|=(T v) noexcept
  {
    v_ |= v;
    return *this;
  }
  constexpr no_init_t &operator^=(T v) noexcept
  {
    v_ ^= v;
    return *this;
  }
  constexpr no_init_t &operator>>=(T v) noexcept
  {
    v_ >>= v;
    return *this;
  }
  constexpr no_init_t &operator<<=(T v) noexcept
  {
    v_ <<= v;
    return *this;
  }

private:
  T v_;
};

