/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_ARGUMENT_HPP_
#define MEXCPPCLASS_MATLAB_ARGUMENT_HPP_

namespace mexcppclass {
namespace matlab {

/*! \brief Define type of argmuent
 */
enum class Argument {
  Input,  /*!< input arguments */
  Output  /*!< output arguments */
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_ARGUMENT_HPP_
