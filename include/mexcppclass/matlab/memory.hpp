/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MEMORY_HPP_
#define MEXCPPCLASS_MATLAB_MEMORY_HPP_

#include <mex.h>
#include <memory>

namespace mexcppclass {
namespace matlab {

class MxArrayDeleter {
 public:
  void operator()(mxArray* ptr) { mxDestroyArray(ptr); }
};

typedef std::unique_ptr<mxArray, MxArrayDeleter> MxArrayUniquePtr;

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MEMORY_HPP_
