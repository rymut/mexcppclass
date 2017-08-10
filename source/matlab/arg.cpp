/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/arg.hpp"

// C++ standard includes
#include <string>
// Project includes
#include "mexcppclass/matlab/argument.hpp"
#include "mexcppclass/utility/string.hpp"

namespace mexcppclass {
namespace matlab {

std::string ArgsName(const Argument &arg) {
  if (arg == Argument::Input) {
    return "argin";
  }
  return "argout";
}

std::string ArgName(const Argument &arg, const int &pos) {
  return utility::FormatString("%s(%d)", ArgsName(arg).c_str(), pos + 1);
}

}  // namespace matlab
}  // namespace mexcppclass
