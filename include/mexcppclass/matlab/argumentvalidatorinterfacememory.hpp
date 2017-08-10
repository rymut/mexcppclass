/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACEMEMORY_HPP_
#define MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACEMEMORY_HPP_

#include <memory>
#include "mexcppclass/matlab/argumentvalidatorinterface.hpp"

namespace mexcppclass {
namespace matlab {
/*! \brief Argument validator unique pointer type
 */
typedef std::unique_ptr<ArgumentValidatorInterface>
  ArgumentValidatorInterfaceUniquePtr;

/*! \brief Smart argument validator shared pointer type
 */
typedef std::shared_ptr<ArgumentValidatorInterface>
  ArgumentValidatorInterfaceSharedPtr;

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ARGUMENTVALIDATORINTERFACEMEMORY_HPP_
