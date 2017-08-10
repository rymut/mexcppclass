/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/messagedeque.hpp"

#include <string>
#include <map>

namespace mexcppclass {
namespace matlab {

void MessageDeque::Display(const MsgLevel &level) const {
  for (auto &it : *this) {
    it.Display(level);
  }
}

void MessageDeque::Display(const MsgLevel &level,
  const std::string &component) const {
  for (auto &it : *this) {
    Message::Display(it.GetDetails(),
      (component.empty()?it.GetComponent():component), it.GetMsgId(), level);
  }
}


}  // namespace matlab
}  // namespace mexcppclass
