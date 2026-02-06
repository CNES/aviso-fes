// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/detail/markdown_table.hpp
/// @brief Markdown table builder for settings and constituents information.
#pragma once
#include <string>
#include <vector>

namespace fes {
namespace detail {

/// @brief Reusable Markdown table builder.
class MarkdownTable {
 public:
  /// @brief Constructor
  /// @param headers[in] The list of column headers for the table
  MarkdownTable(std::initializer_list<std::string> headers)
      : headers_(headers) {}

  /// @brief Add a row to the table
  /// @param values[in] The list of values for the row, must have the same
  /// number of elements as the number of headers (no check is performed)
  auto add_row(std::initializer_list<std::string> values) -> void {
    rows_.emplace_back(values);
  }

  /// @brief Convert the table to a Markdown-formatted string
  /// @return A string containing the Markdown representation of the table
  auto to_string() const -> std::string {
    std::string result;
    // Header row
    result += format_row(headers_);
    // Separator row
    result += separator();
    // Data rows
    for (const auto& row : rows_) {
      result += format_row(row);
    }
    return result;
  }

 private:
  /// @brief Format a row of the table as a Markdown string
  /// @param cells[in] The list of cell values for the row
  /// @return A string containing the Markdown representation of the row
  static auto format_row(const std::vector<std::string>& cells) -> std::string {
    std::string row = "|";
    for (const auto& cell : cells) {
      row += " " + cell + " |";
    }
    row += "\n";
    return row;
  }

  /// @brief Generate the separator row for the Markdown table
  /// @return A string containing the Markdown representation of the separator
  /// row
  auto separator() const -> std::string {
    std::string sep = "|";
    for (const auto& header : headers_) {
      // Use at least 3 dashes, or match header width + 2 (for the spaces)
      auto width = header.size() < 3 ? 3 : header.size();
      sep += std::string(width + 2, '-') + "|";
    }
    sep += "\n";
    return sep;
  }

  /// @brief The list of column headers for the table
  std::vector<std::string> headers_;
  /// @brief The list of rows for the table, where each row is a list of cell
  /// values
  std::vector<std::vector<std::string>> rows_;
};

}  // namespace detail
}  // namespace fes
