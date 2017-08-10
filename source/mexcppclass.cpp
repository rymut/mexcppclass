/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/core/main.hpp"

void mexFunction(int output_count, mxArray *output[],
  int input_count, const mxArray *input[]) {
  mexcppclass::MexFunction(input_count, input, output_count, output);
}
