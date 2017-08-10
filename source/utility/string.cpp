/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

// Implementation definition include
#include "mexcppclass/utility/string.hpp"
// C++ standard includes
#include <cctype>
#include <string>

namespace mexcppclass {
namespace utility {

std::string ToLowerCase(const std::string &text) {
  std::string result = text;
  for (auto & c : result) {
    c = ::tolower(c);
  }
  return result;
}

bool IsAlphaNumeric(const std::string &text) {
  for (auto c : text) {
    if (!isalnum(c)) {
      return false;
    }
  }
  return true;
}

}  // namespace utility
}  // namespace mexcppclass
