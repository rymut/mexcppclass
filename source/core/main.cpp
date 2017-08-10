/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/core/main.hpp"

#include <mex.h>
#include "core/program.hpp"

namespace mexcppclass {

void MexFunction(const int &input_count, const mxArray *input[],
  const int &output_count, mxArray *output[]) {
  static mexcppclass::Program program;
  program.Main(input_count, input, output_count, output);
}

}  // namespace mexcppclass
