/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_STRING_HPP_
#define MEXCPPCLASS_MATLAB_STRING_HPP_

#include <matrix.h>
#include <string>

namespace mexcppclass {
namespace matlab {

std::string StringFromMxArray(const mxArray *array);

std::string StringFunctionName();

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_STRING_HPP_
