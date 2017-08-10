/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_MESSAGE_HPP_
#define MEXCPPCLASS_MATLAB_MESSAGE_HPP_

#include <string>
#include "mexcppclass/matlab/msgid.hpp"
#include "mexcppclass/matlab/msglevel.hpp"

namespace mexcppclass {
namespace matlab {

/*! \brief Matlab message
 */
class Message {
 public:
  Message(const MsgId &msg_id,
    const std::string &component = "unknown",
    const std::string &details = "");

  MsgId GetMsgId() const;

  std::string GetDetails() const;

  std::string GetComponent() const;

  void Display(const MsgLevel &level) const;

  static void Display(const std::string &details,
    const std::string &component = "", const MsgId &msg_id = MsgId::None,
    const MsgLevel &level = MsgLevel::Information);

 private:
  MsgId msg_id_;
  std::string details_;
  std::string component_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_MESSAGE_HPP_
