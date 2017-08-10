/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_CELL_HPP_
#define MEXCPPCLASS_MATLAB_CELL_HPP_

#include <matrix.h>
#include <vector>

namespace mexcppclass {
namespace matlab {

template <typename T>
mxArray *CellFromVector(const std::vector<typename T> &vec,
  mxArray *(*convert_func_ptr)(typename const T &)) {
  const mwSize size = static_cast<mwSize>(vec.size());
  mxArray *result = mxCreateCellMatrix(1LL, size);
  if (result == nullptr || convert_func_ptr == nullptr) {
    return result;
  }
  for (mwIndex idx = 0; idx < size; ++idx) {
    mxSetCell(result, idx, convert_func_ptr(vec[idx]));
  }
  return result;
}

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_CELL_HPP_
