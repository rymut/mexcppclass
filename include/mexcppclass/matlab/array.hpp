/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ARRAY_HPP_
#define MEXCPPCLASS_MATLAB_ARRAY_HPP_

#include <mex.h>
#include <string>

namespace mexcppclass {
namespace matlab {

mxArray *ArrayFromString(const std::string &value);

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ARRAY_HPP_
