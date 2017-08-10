/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/numeric.hpp"

#include <mex.h>
#include <matrix.h>
#include <vector>

namespace mexcppclass {
namespace matlab {

bool MxIsNumeric(const mxArray *array) {
  const std::vector<mxClassID> numeric = {
    mxDOUBLE_CLASS, mxSINGLE_CLASS, mxINT8_CLASS, mxUINT8_CLASS,
    mxINT16_CLASS, mxUINT16_CLASS, mxINT32_CLASS, mxUINT32_CLASS,
    mxINT64_CLASS, mxUINT64_CLASS,
  };
  if (array == nullptr) {
    return false;
  }
  return numeric.end() !=
    std::find(numeric.begin(), numeric.end(), mxGetClassID(array));
}

}  // namespace matlab
}  // namespace mexcppclass
