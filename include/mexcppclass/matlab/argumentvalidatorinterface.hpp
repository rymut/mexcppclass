/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACE_HPP_
#define MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACE_HPP_

#include <mex.h>
#include "mexcppclass/matlab/messagedeque.hpp"

namespace mexcppclass {
namespace matlab {

/*! \brief Interface for validator
 */
class ArgumentValidatorInterface {
 public:
  /*! \brief Pure virtual interface
  */
  virtual ~ArgumentValidatorInterface() = 0;

  /*! \brief Check if arguments are valid
  *  \param nlhs - number of left hand side arguments
  *  \param plhs - left hand side arguments
  *  \param nrhs - number of right hand side arguments
  *  \param plhs - right hand side arguments
  *  \return true if argument/arguments are valid
  */
  virtual bool IsValid(
    const int &input_count, const mxArray * const * const input,
    const int output_count, const mxArray * const * const output,
    MessageDeque &deque = MessageDeque()) const { return false; }
};

inline ArgumentValidatorInterface::~ArgumentValidatorInterface() {
}

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACE_HPP_
