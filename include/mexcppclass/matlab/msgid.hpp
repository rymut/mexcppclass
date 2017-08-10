/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MSGID_HPP_
#define MEXCPPCLASS_MATLAB_MSGID_HPP_

namespace mexcppclass {
namespace matlab {

/*! \brief Enum defining message id for mex errors/warnings
 */
enum class MsgId {
  MemoryAllocationFailure,  /*!< Memory allocation failure */
  NotEnoughInputs,  /*< not enought input arguments */
  ToManyInputs,  /*< to many input arguments */

  NotEnoughOutputs,  /*< not enought output argumunts */
  ToManyOutputs,  /*< to many output arguments */

  NotEnoughDimensions,  /*!< not enought dimensions */
  ToManyDimensions,  /*!< to many dimensions */

  DimensionLenghtToSmall,  /*!< dimension lenght to small */
  DimensionLenghtToBig,  /*!< dimension lenght to big */

  InvalidInput, /*!< invalid input arguments */
  InvalidOutput, /*!< invalid output arguments */
  InvalidArguments, /*!< invalid input output arguments */
  InvalidArgument,  /*!< invalid argument */

  InvalidCommand,  /*!< invalid command */
  InvalidHandle,  /*!< invalid handle */
  InvalidType,  /*!< invalid type */
  InvalidClassId, /*!< invalid class id */
  InvalidMethod,  /*!< invalid method */

  RequiredArgumentMissing,  /*< required argument */
  Custom,  /*< custom message */
  NotImplemented,  /*< not implemented */
  None
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MSGID_HPP_
