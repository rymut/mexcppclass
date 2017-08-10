/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MSGTEXT_HPP_
#define MEXCPPCLASS_MATLAB_MSGTEXT_HPP_

#include <string>
#include "mexcppclass/matlab/msgid.hpp"

namespace mexcppclass {
namespace matlab {

std::string MsgTextName(const MsgId &id);

std::string MsgTextDesc(const MsgId &id);

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MSGTEXT_HPP_
