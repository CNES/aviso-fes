// Copyright (c) 2025 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/isviewstream.hpp
/// @brief String view stream.
#pragma once
#include <algorithm>
#include <istream>
#include <streambuf>

#include "fes/string_view.hpp"

namespace fes {
namespace detail {

/// string_view-compatible buffer for reading read-only characters from memory.
/// Implements a buffer based on stackoverflow discussions. Written to be used
/// via isviewstream.
///
/// @warning sviewbuf does not own memory. If the lifetime of the string ends
/// before this buffer, reading from it invokes undefined behavior.
///
/// @see https://stackoverflow.com/a/13059195
/// @see https://stackoverflow.com/a/46069245
/// @see https://gist.github.com/andreasxp/ac9adcf8a2b37ac05ff7047f8728b3c7
class sviewbuf : public std::streambuf {
 public:
  /// @brief Construct a string view buffer from a null-terminated string.
  /// @param[in] s The null-terminated string.
  /// @param[in] count The size of the string.
  sviewbuf(const char* s, std::size_t count) {
    auto* begin = const_cast<char*>(s);  // NOLINT
    this->setg(begin, begin, begin + count);
  }

  /// @brief Construct a string view buffer from a string view.
  /// @param[in] str The string view.
  explicit sviewbuf(const string_view& str)
      : sviewbuf(str.data(), str.size()) {}

 protected:
  /// @brief Seek to a position in the buffer.
  /// @param[in] off Offset to seek to.
  /// @param[in] dir Direction to seek.
  /// @return The position in the buffer.
  auto seekoff(off_type off, std::ios_base::seekdir dir,
               std::ios_base::openmode /*which*/) -> pos_type override {
    if (dir == std::ios_base::cur) {
      gbump(static_cast<int>(off));
    } else if (dir == std::ios_base::end) {
      setg(eback(), egptr() + off, egptr());
    } else if (dir == std::ios_base::beg) {
      setg(eback(), eback() + off, egptr());
    }
    return gptr() - eback();
  }

  /// @brief Seek to a position in the buffer.
  /// @param[in] sp The position to seek to.
  /// @param[in] which The open mode.
  /// @return The position in the buffer.
  auto seekpos(pos_type sp, std::ios_base::openmode which)
      -> pos_type override {
    return seekoff(sp - pos_type(static_cast<off_type>(0)), std::ios_base::beg,
                   which);
  }
};

/// A std::stringstream analog for string_view.
/// Implements a string_view-based input stream based on stackoverflow
/// discussions.
///
/// @warning isviewstream does not own memory. If the lifetime of the string
/// ends before this stream, reading from it invokes undefined behavior.
class isviewstream : private virtual sviewbuf, public std::istream {
 public:
  /// @brief Construct a string view stream from a string view.
  /// @param[in] str The string view.
  explicit isviewstream(const string_view& str)
      : sviewbuf(str), std::istream(static_cast<std::streambuf*>(this)) {}

  /// @brief Extract a string_view from the stream.
  /// @param[in] count The number of characters to extract.
  /// @return The string_view.
  auto readview(const std::streamsize count) -> string_view {
    auto avail = this->egptr() - this->gptr();
    auto size = std::min(count, avail);
    auto result = string_view(gptr(), size);
    seekoff(size, std::ios_base::cur, std::ios_base::in);
    if (avail < count) {
      setstate(std::ios_base::badbit | std::ios_base::failbit);
    }
    return result;
  }
};

}  // namespace detail
}  // namespace fes
