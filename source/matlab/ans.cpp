/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

// implementation declaration
#include "mexcppclass/matlab/ans.hpp"
// C system files
#include <mex.h>
// C++ system files
#include <functional>
#include <memory>
// Other includes
// Project includes
#include "mexcppclass/matlab/memory.hpp"

namespace mexcppclass {
namespace matlab {

void SetAns(mxArray *value) {
  if (!mexPutVariable("caller", "ans", value)) {
// cannot be silenced    mexEvalString("ans");
  }
}

}  // namespace matlab
}  // namespace mexcppclass
