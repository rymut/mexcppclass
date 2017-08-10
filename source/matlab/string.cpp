/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

// Implementation definition inclue
#include "mexcppclass/matlab/string.hpp"
// C standard includes
#include <mex.h>
// C++ standard headers
#include <string>

namespace mexcppclass {
namespace matlab {

std::string StringFromMxArray(const mxArray *array) {
  std::string result = "";
  if (array != nullptr) {
    char *text = mxArrayToString(array);
    if (text != nullptr) {
      result = std::string(text);
      mxFree(text);
    }
    text = nullptr;
  }
  return result;
}

std::string StringFunctionName() {
  return std::string(mexFunctionName());
}

}  // namespace matlab
}  // namespace mexcppclass
