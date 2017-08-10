/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MSGLEVEL_HPP_
#define MEXCPPCLASS_MATLAB_MSGLEVEL_HPP_

namespace mexcppclass {
namespace matlab {

/*! \brief Matlab message level
 */
enum class MsgLevel {
  Information, /*!< Show information and continue program execution  */
  Warning,  /*!< Issue warning and continue program execution */
  Error  /*!< Throw error and stop program execution */
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MSGLEVEL_HPP_
