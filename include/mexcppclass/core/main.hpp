/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_MAIN_HPP_
#define MEXCPPCLASS_CORE_MAIN_HPP_

#include <mex.h>

namespace mexcppclass {

/*! \brief Mex entry point
 *  Arguments are 'class_name', 'action', action arguments
 *
 *  Return class names without any parameters - not possible
 *    without getting proc addresses.
 *  Return method names with single argument if class exists.
 *  Return object or call method when two arguments are passed.
 *
 *  \param nlhs - number of left hand side parameters
 *  \param plhs - left hand side parameters
 *  \param nrhs - number of right hand side parameters
 *  \param mxArray - right hand size arguments
 */
void MexFunction(const int &nrhs, const mxArray *prhs[],
  const int &nlhs, mxArray *plhs[]);

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_MAIN_HPP_
