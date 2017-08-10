/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ARG_HPP_
#define MEXCPPCLASS_MATLAB_ARG_HPP_

// C++ standard includes
#include <string>
// Project includes
#include "mexcppclass/matlab/argument.hpp"

namespace mexcppclass {
namespace matlab {

std::string ArgsName(const Argument &arg);

std::string ArgName(const Argument &arg, const int &pos);

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ARG_HPP_
