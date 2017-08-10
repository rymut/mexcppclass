/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ANS_HPP_
#define MEXCPPCLASS_MATLAB_ANS_HPP_

#include <mex.h>

namespace mexcppclass {
namespace matlab {

void SetAns(mxArray *value);

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ANS_HPP_
