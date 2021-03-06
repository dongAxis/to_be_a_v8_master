// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_STRING_FORMAT_H_
#define V8_BASE_STRING_FORMAT_H_

#include <array>
#include <limits>
#include <string_view>
#include <tuple>

#include "src/base/logging.h"
#include "src/base/platform/platform.h"

namespace v8::base {

// Implementation detail, do not use outside this header. The public interface
// is below.
namespace impl {

template <const std::string_view&... strs>
struct JoinedStringViews {
  static constexpr auto JoinIntoNullTerminatedArray() noexcept {
    constexpr size_t kArraySize = (1 + ... + strs.size());
    std::array<char, kArraySize> arr{};
    char* ptr = arr.data();
    for (auto str : std::initializer_list<std::string_view>{strs...}) {
      for (auto c : str) *ptr++ = c;
    }
    *ptr++ = '\0';
    DCHECK_EQ(arr.data() + arr.size(), ptr);
    return arr;
  }

  // Store in an array with static linkage, so we can reference it from the
  // {std::string_view} below.
  static constexpr auto array = JoinIntoNullTerminatedArray();

  // Create a string view to the null-terminated array. The null byte is not
  // included.
  static constexpr std::string_view string_view = {array.data(),
                                                   array.size() - 1};
};

template <typename T>
struct FormattedStringPart {
  static_assert(sizeof(T) < 0,
                "unimplemented type, add specialization below if needed");
};

template <>
struct FormattedStringPart<int> {
  // Integer range: [-2147483647, 2147483647]. Representable in 11 characters.
  static constexpr int kMaxLen = 11;
  static constexpr std::string_view kFormatPart = "%d";

  int value;
};

template <>
struct FormattedStringPart<size_t> {
  // size_t range: [0, 4294967295] on 32-bit, [0, +18446744073709551615] on
  // 64-bit. Needs 10 or 20 characters.
  static constexpr int kMaxLen = sizeof(size_t) == sizeof(uint32_t) ? 10 : 20;
  static constexpr std::string_view kFormatPart = "%zu";

  size_t value;
};

template <size_t N>
struct FormattedStringPart<char[N]> {
  static_assert(N >= 1, "Do not print (static) empty strings");
  static_assert(N <= 128, "Do not include huge strings");
  static constexpr int kMaxLen = N - 1;
  static constexpr std::string_view kFormatPart = "%s";

  const char* value;
};

template <const std::string_view& kFormat, int kMaxLen, typename... Parts>
std::array<char, kMaxLen> PrintFormattedStringToArray(Parts... parts) {
  std::array<char, kMaxLen> message;

  static_assert(kMaxLen > 0);
  static_assert(
      kMaxLen < 128,
      "Don't generate overly large strings; this limit can be increased, but "
      "consider that the array lives on the stack of the caller.");

  // Add a special case for empty strings, because compilers complain about
  // empty format strings.
  static_assert((kFormat.size() == 0) == (sizeof...(Parts) == 0));
  if constexpr (kFormat.size() == 0) {
    message[0] = '\0';
  } else {
    int characters = base::OS::SNPrintF(message.data(), kMaxLen, kFormat.data(),
                                        parts.value...);
    CHECK(characters >= 0 && characters < kMaxLen);
    DCHECK_EQ('\0', message[characters]);
  }

  return message;
}

}  // namespace impl

// `FormattedString` allows to format strings with statically known number and
// type of constituents.
// The class stores all values that should be printed, and generates the final
// string via `SNPrintF` into a `std::array`, without any dynamic memory
// allocation. The format string is computed statically.
// This makes this class not only very performant, but also suitable for
// situations where we do not want to perform any memory allocation (like for
// reporting OOM or fatal errors).
//
// Use like this:
//   auto message = FormattedString{} << "Cannot allocate " << size << " bytes";
//   V8::FatalProcessOutOfMemory(nullptr, message.PrintToArray().data());
//
// This code is compiled into the equivalent of
//   std::array<char, 34> message_arr;
//   int chars = SNPrintF(message_arr.data(), 34, "%s%d%s", "Cannot allocate ",
//                        size, " bytes");
//   CHECK(chars >= 0 && chars < 34);
//   V8::FatalProcessOutOfMemory(nullptr, message_arr.data());
template <typename... Ts>
class FormattedString {
  template <typename T>
  using Part = impl::FormattedStringPart<T>;

  static_assert(std::conjunction_v<std::is_trivial<Part<Ts>>...>,
                "All parts needs to be trivial to guarantee optimal code");

 public:
  static constexpr int kMaxLen = (1 + ... + Part<Ts>::kMaxLen);
  static constexpr std::string_view kFormat =
      impl::JoinedStringViews<Part<Ts>::kFormatPart...>::string_view;

  FormattedString() {
    static_assert(sizeof...(Ts) == 0,
                  "Only explicitly construct empty FormattedString, use "
                  "operator<< to appending");
  }

  // Add one more part to the FormattedString. Only allowed on r-value ref (i.e.
  // temporary object) to avoid misuse like `FormattedString<> str; str << 3;`
  // instead of `auto str = FormattedString{} << 3;`.
  template <typename T>
  V8_WARN_UNUSED_RESULT auto operator<<(T&& t) const&& {
    using PlainT = std::remove_cv_t<std::remove_reference_t<T>>;
    return FormattedString<Ts..., PlainT>{
        std::tuple_cat(parts_, std::make_tuple(Part<PlainT>{t}))};
  }

  // Print this FormattedString into an array. Does not allocate any dynamic
  // memory. The result lives on the stack of the caller.
  V8_INLINE V8_WARN_UNUSED_RESULT std::array<char, kMaxLen> PrintToArray()
      const {
    return std::apply(
        impl::PrintFormattedStringToArray<kFormat, kMaxLen, Part<Ts>...>,
        parts_);
  }

 private:
  template <typename... Us>
  friend class FormattedString;

  explicit FormattedString(std::tuple<Part<Ts>...> parts) : parts_(parts) {}

  std::tuple<Part<Ts>...> parts_;
};

}  // namespace v8::base

#endif  // V8_BASE_STRING_FORMAT_H_
