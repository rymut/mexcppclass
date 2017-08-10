/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/array.hpp"

#include <mex.h>
#include <matrix.h>
#include <string>

namespace mexcppclass {
namespace matlab {

mxArray *ArrayFromString(const std::string &value) {
  return mxCreateString(value.c_str());
}

}  // namespace matlab
}  // namespace mexcppclass
