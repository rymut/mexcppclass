/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/message.hpp"

#include <mex.h>

#include <string>
#include "mexcppclass/matlab/msgid.hpp"
#include "mexcppclass/matlab/msglevel.hpp"
#include "mexcppclass/matlab/msgtext.hpp"

namespace mexcppclass {
namespace matlab {

Message::Message(const MsgId &msg_id,
  const std::string &component,
  const std::string &details) {
  this->msg_id_ = msg_id;
  this->details_ = details;
  this->component_ = component;
}

MsgId Message::GetMsgId() const {
  return this->msg_id_;
}
std::string Message::GetDetails() const {
  return this->details_;
}
std::string Message::GetComponent() const {
  return this->component_;
}

void Message::Display(const MsgLevel &level) const {
  Message::Display(this->details_, this->component_, this->msg_id_, level);
}

void Message::Display(const std::string &details,
  const std::string &component, const MsgId &msg_id, const MsgLevel &level) {
  std::string name = MsgTextName(msg_id);
  std::string desc = MsgTextDesc(msg_id);

  if (level == MsgLevel::Warning) {
    mexWarnMsgIdAndTxt(
      ((component.empty()?"":component+":") + name).c_str(),
      (desc + (details.empty()?".":": "+details+".")).c_str() );
  } else if (level == MsgLevel::Error) {
    mexErrMsgIdAndTxt(
      ((component.empty()?"":component+":") + name).c_str(),
      (desc + (details.empty()?".":": "+details+".")).c_str() );
  } else {
    mexPrintf((name + " " + component + " " + details).c_str());
  }
}

}  // namespace matlab
}  // namespace mexcppclass
