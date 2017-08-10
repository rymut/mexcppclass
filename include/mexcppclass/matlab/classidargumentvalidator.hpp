/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_CLASSIDARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_CLASSIDARGUMENTVALIDATOR_HPP_

#include <mex.h>
#include "mexcppclass/matlab/classid.hpp"
#include "mexcppclass/matlab/existargumentvalidator.hpp"

namespace mexcppclass {
namespace matlab {

class ClassIdArgumentValidator : public ExistArgumentValidator  {
 public:
  ClassIdArgumentValidator(const ClassId &type, const int &position);

  virtual bool IsValid(
    const int &nrhs, const mxArray * const * const prhs,
    const int nlhs, const mxArray * const * const plhs,
    MessageDeque &deque = MessageDeque()) const;

 private:
  ClassId class_type_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_CLASSIDARGUMENTVALIDATOR_HPP_
