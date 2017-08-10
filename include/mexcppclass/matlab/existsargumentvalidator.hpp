/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_EXISTSARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_EXISTSARGUMENTVALIDATOR_HPP_

#include <string>
#include "mexcppclass/matlab/argumentvalidatorinterface.hpp"
#include "mexcppclass/matlab/argument.hpp"

namespace mexcppclass {
namespace matlab {

class ExistsArgumentValidator : public ArgumentValidatorInterface {
 public:
  explicit ExistsArgumentValidator(const Argument &side);

  ~ExistsArgumentValidator();

  const mxArray *const *const GetArguments(const mxArray *const *const input,
    const mxArray *const *const output) const;

  int GetArgumentsNumber(const int &input_count,
    const int &output_count) const;

  virtual bool IsValid(
    const int &input_count, const mxArray * const * const input,
    const int output_count, const mxArray * const * const output,
    MessageDeque &list = MessageDeque()) const;

 protected:
  Argument GetArgument() const;

  std::string GetArgsName() const;

 private:
  Argument side_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_EXISTSARGUMENTVALIDATOR_HPP_
