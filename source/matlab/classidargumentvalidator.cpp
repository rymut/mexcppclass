/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/classidargumentvalidator.hpp"

#include <matrix.h>
#include <map>
#include <vector>
#include "mexcppclass/matlab/classid.hpp"
#include "mexcppclass/matlab/numeric.hpp"

namespace mexcppclass {
namespace matlab {

ClassIdArgumentValidator::ClassIdArgumentValidator(const ClassId &type,
  const int &position) : ExistArgumentValidator(Argument::Input, position) {
  this->class_type_ = type;
}

bool ClassIdArgumentValidator::IsValid(
  const int &nrhs, const mxArray * const * const prhs,
  const int nlhs, const mxArray * const * const plhs,
  MessageDeque &deque) const {
  // \TODO (rymut): not full enumeration settings
  const std::map<ClassId, bool (*)(const mxArray *)> class_test_map = {
    { ClassId::Single, mxIsSingle },
    { ClassId::Double, mxIsDouble },
    { ClassId::Int64, mxIsInt64 },
    { ClassId::Uint64, mxIsUint64 },
    { ClassId::Int32, mxIsInt32 },
    { ClassId::Uint32, mxIsUint32 },
    { ClassId::Int16, mxIsInt16 },
    { ClassId::Uint16, mxIsUint16 },
    { ClassId::Int8, mxIsInt8 },
    { ClassId::Uint8, mxIsUint8 },
    { ClassId::Scalar, mxIsScalar },
    { ClassId::Char, mxIsChar },
    { ClassId::Logical, mxIsLogical },
    { ClassId::Cell, mxIsCell },
    { ClassId::Numeric, matlab::MxIsNumeric }
  };
  if (ExistArgumentValidator::IsValid(nrhs, prhs, nlhs, plhs, deque) == false) {
    return false;
  }

  auto argument = this->GetArgument(nrhs, prhs, nlhs, plhs);
  if (argument == nullptr) {
    return false;
  }
  // \TODO (rymut): message message cast
  auto it = class_test_map.find(this->class_type_);
  if (it != class_test_map.end()) {
    bool result = it->second(argument);
    if (result == false) {
      deque.push_back(Message(MsgId::InvalidInput, "class",
        "wrong class of " + this->GetArgName()));
    }
    return result;
  }
  deque.push_back(Message(MsgId::NotImplemented, "class",
    "selected class validator not implemented"));
  // \TODO (rymut): warring unknown type to try
  // cannot validate
  return false;
}

}  // namespace matlab
}  // namespace mexcppclass
