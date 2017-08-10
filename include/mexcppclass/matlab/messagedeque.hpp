/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MESSAGEDEQUE_HPP_
#define MEXCPPCLASS_MATLAB_MESSAGEDEQUE_HPP_

#include <deque>
#include <string>

#include "mexcppclass/matlab/message.hpp"
#include "mexcppclass/matlab/msglevel.hpp"

namespace mexcppclass {
namespace matlab {
/*! \brief Message queue type definition
 */
class MessageDeque : public std::deque<Message> {
 public:
  void Display(const MsgLevel &level) const;
  void Display(const MsgLevel &level, const std::string &component) const;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MESSAGEDEQUE_HPP_
