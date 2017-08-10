/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/countargumentvalidator.hpp"
#include "mexcppclass/matlab/existsargumentvalidator.hpp"
#include "mexcppclass/utility/string.hpp"

namespace mexcppclass {
namespace matlab {

CountArgumentValidator::CountArgumentValidator(const Argument &side,
  const int &minimum, const int &maximum) : ExistsArgumentValidator(side) {
  this->minimum_ = minimum;
  this->maximum_ = maximum;
}

CountArgumentValidator::~CountArgumentValidator() {
}

bool CountArgumentValidator::IsValid(const int &nrhs,
  const mxArray * const * const prhs,
  const int nlhs, const mxArray * const * const plhs,
  MessageDeque &deque) const {
  const int argc = this->GetArgumentsNumber(nrhs, nlhs);

  if (argc < this->minimum_) {
    MsgId msg_id = MsgId::NotEnoughInputs;
    if (this->GetArgument() == Argument::Output) {
      msg_id = MsgId::NotEnoughOutputs;
    }
    deque.push_back(Message(msg_id, "count",
      utility::FormatString("%s element count must be "
        "equal or greater than %d", this->GetArgsName(), this->minimum_)));
    return false;
  }
  if (argc > this->maximum_) {
    MsgId msg_id = MsgId::ToManyInputs;
    if (this->GetArgument() == Argument::Output) {
      msg_id = MsgId::ToManyOutputs;
    }
    deque.push_back(Message(msg_id, "count",
      utility::FormatString("%s element count must be "
        "equal or smaller than %d", this->GetArgsName(), this->maximum_)));
    return false;
  }
  return true;
}

}  // namespace matlab
}  // namespace mexcppclass
