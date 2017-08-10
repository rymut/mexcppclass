/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/program.hpp"

#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "mexcppclass/utility/string.hpp"
#include "mexcppclass/matlab/string.hpp"
#include "mexcppclass/utility/version.hpp"
#include "mexcppclass/matlab/ans.hpp"
#include "mexcppclass/matlab/memory.hpp"
#include "mexcppclass/matlab/cell.hpp"
#include "mexcppclass/matlab/array.hpp"
#include "mexcppclass/matlab/arg.hpp"
#include "mexcppclass/matlab/argument.hpp"
#include "mexcppclass/moduleinterface.hpp"
#include "mexcppclass/matlab/argumentvalidatorinterfacememory.hpp"
#include "mexcppclass/matlab/countargumentvalidator.hpp"
#include "mexcppclass/matlab/dimensionscountargumentvalidator.hpp"
#include "mexcppclass/matlab/classidargumentvalidator.hpp"

#include "core/persistentdata.hpp"
#include "core/nonpersistentdata.hpp"

namespace mexcppclass {

mxArray *ArrayFromInstanceId(const InstanceId &value) {
  return mxCreateDoubleScalar(static_cast<double>(value));
}

/*! \brief Command entry for program
 */
typedef void (Program::*CommandEntryPoint)(NonpersistentDataInterface *);

static const std::map<std::string, CommandEntryPoint> kCommandMap = {
  { "create", &Program::CommandCreate },
  { "destroy", &Program::CommandDestroy },
  { "handle", &Program::CommandHandle },
  { "methods", &Program::CommandMethods },
  { "call", &Program::CommandCall },
  { "type", &Program::CommandType },
  { "objects", &Program::CommandObjects },

  { "commands", &Program::CommandCommands },
  { "version", &Program::CommandVersion },
  { "libpath", &Program::CommandLibpath },
  { "clear", &Program::CommandClear },

  { "types", &Program::CommandTypes }
};

Program::Program() {
}

Program::~Program() {
  this->instance_mgr_.Clear();
  this->module_mgr_.GarbageCollect();
}

void Program::CommandLibpath(NonpersistentDataInterface *args) {
  if (args == nullptr) {
    return;
  }

  const int kArgInMinCount = args->ArgInOffset();
  const int kArgInMaxCount = kArgInMinCount + 2;
  const int kArgInAction = args->ArgInOffset();
  const int kArgInPath = kArgInAction + 1;
  const int kArgOutMinCount = args->ArgOutOffset();
  const int kArgOutMaxCount = kArgOutMinCount + 1;
  const int kArgOutPath = args->ArgOutOffset();

  const auto default_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset(), args->ArgInOffset())),
  };

  const auto action_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinCount + 1, (std::numeric_limits<int>::max)())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        kArgInAction)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::DimensionsCountArgumentValidator(kArgInAction, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };
  const auto restore_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinCount + 1, kArgInMinCount + 1))
  };
  const auto add_del_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMaxCount, kArgInMaxCount)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        kArgInAction)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::DimensionsCountArgumentValidator(kArgInAction, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()})),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        kArgInPath)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::DimensionsCountArgumentValidator(kArgInPath, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };

  enum class Validator {
    kDefault,
    kAction,
    kRestore,
    kAddOrDel
  };
  std::map<Validator,
    std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>>
    exec_path_validators = {
    { Validator::kDefault, default_validators },
    { Validator::kAction, action_validators },
    { Validator::kRestore, restore_validators },
    { Validator::kAddOrDel, add_del_validators }
  };

  std::map<Validator, matlab::MessageDeque> validator_deque = {
    { Validator::kDefault, matlab::MessageDeque() },
    { Validator::kAction, matlab::MessageDeque() },
    { Validator::kRestore, matlab::MessageDeque() },
    { Validator::kAddOrDel, matlab::MessageDeque() }
  };

  for (auto &validators : exec_path_validators) {
    for (auto &validator : validators.second) {
      if (validator.get() != nullptr && validator->IsValid(
        args->ArgInCount(), args->ArgIn(), args->ArgOutCount(), args->ArgOut(),
        validator_deque[validators.first]) == false) {
        break;
      }
    }
  }

  if (validator_deque[Validator::kDefault].size() == 0) {
    // everything ok
  } else if (validator_deque[Validator::kAction].size() == 0) {
    const std::string &action = utility::ToLowerCase(matlab::StringFromMxArray(
      args->ArgIn()[kArgInAction]));

    if (action == "restore") {
      this->module_mgr_.RestoreLibrarySearchPath();
    } else if (action == "add") {
      if (validator_deque[Validator::kAddOrDel].size()) {
        validator_deque[Validator::kAddOrDel].Display(matlab::MsgLevel::Error,
          matlab::StringFunctionName() + ":libpath");
        return;
      }
      this->module_mgr_.InsertLibrarySearchPath(matlab::StringFromMxArray(
        args->ArgIn()[kArgInPath]));
    } else if (action == "del") {
      if (validator_deque[Validator::kAddOrDel].size()) {
        validator_deque[Validator::kAddOrDel].Display(matlab::MsgLevel::Error,
          matlab::StringFunctionName() + ":libpath");
        return;
      }
      this->module_mgr_.EraseLibrarySearchPath(matlab::StringFromMxArray(
        args->ArgIn()[kArgInPath]));
    } else {
      matlab::Message(matlab::MsgId::InvalidArguments,
        matlab::StringFunctionName() + ":libpath").Display(
          matlab::MsgLevel::Error);
      return;
    }
  } else {
    validator_deque[Validator::kAction].Display(matlab::MsgLevel::Error,
      matlab::StringFunctionName() + ":libpath");
    return;
  }

  mxArray *matrix = matlab::CellFromVector(
    this->module_mgr_.GetLibrarySearchPath(), &matlab::ArrayFromString);
  if (args->ArgOutCount() <= kArgOutPath) {
    matlab::MxArrayUniquePtr result(matrix);
    matlab::SetAns(result.get());
  } else {
    args->ArgOut()[kArgOutPath] = matrix;
  }
}

void Program::CommandCommands(NonpersistentDataInterface *args) {
  if (args == nullptr) {
    return;
  }
  const int kArgInMinMaxCount = args->ArgInOffset();
  const int kArgOutMinCount = args->ArgOutOffset();
  const int kArgOutMaxCount = args->ArgOutOffset() + 1;
  const int kArgOutMethods = args->ArgOutOffset();
  const auto argio_validators = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinMaxCount, kArgInMinMaxCount)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount))
  };
  // validate input and autput
  for (auto &validator : argio_validators) {
    matlab::MessageDeque deque;
    if (validator->IsValid(args->ArgInCount(), args->ArgIn(),
      args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error,
        matlab::StringFunctionName() + ":commands");
      return;
    }
  }
  // create result vector
  std::vector<std::string> commands;
  for (const auto &item : kCommandMap) {
    commands.push_back(item.first);
  }
  // store vector in CELL
  mxArray *matrix = matlab::CellFromVector(commands, matlab::ArrayFromString);
  if (matrix == nullptr) {
    matlab::Message(matlab::MsgId::MemoryAllocationFailure,
      "methods").Display(matlab::MsgLevel::Error);
    return;
  }
  if (args->ArgOutCount() <= kArgOutMethods) {
    matlab::MxArrayUniquePtr result(matrix);
    matlab::SetAns(result.get());
  } else {
    args->ArgOut()[kArgOutMethods] = matrix;
  }
}

void Program::CommandType(NonpersistentDataInterface *args) {
  matlab::Message(matlab::MsgId::NotImplemented).Display(
    matlab::MsgLevel::Error);
}

void Program::CommandTypes(NonpersistentDataInterface *args) {
  matlab::Message(matlab::MsgId::NotImplemented).Display(
    matlab::MsgLevel::Error);
}

void Program::CommandObjects(NonpersistentDataInterface *args) {
  if (args == nullptr) {
    return;
  }
  const int kArgInMinMaxCount = args->ArgInOffset();
  const int kArgOutMinCount = args->ArgOutOffset();
  const int kArgOutMaxCount = kArgOutMinCount + 1;
  const int kArgOutObjects = args->ArgOutOffset();

  const auto argio_validators = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinMaxCount, kArgInMinMaxCount)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount))
  };

  for (const auto &validator : argio_validators) {
    matlab::MessageDeque deque;
    if (validator.get() != nullptr && validator.get()->IsValid(
      args->ArgInCount(), args->ArgIn(),
      args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error,
        matlab::StringFunctionName() +":objects");
      return;
    }
  }

  std::vector<InstanceId> objects = this->instance_mgr_.GetInstances();
  mxArray *matrix = matlab::CellFromVector(objects, &ArrayFromInstanceId);
  if (matrix == nullptr) {
    matlab::Message(matlab::MsgId::MemoryAllocationFailure,
      "methods").Display(matlab::MsgLevel::Error);
    return;
  }
  if (args->ArgOutCount() <= kArgOutObjects) {
    matlab::MxArrayUniquePtr result(matrix);
    matlab::SetAns(result.get());
  } else {
    args->ArgOut()[kArgOutObjects] = matrix;
  }
}

void Program::Main(const int &input_count, const mxArray * const * const input,
  const int &output_count, mxArray **output) {
  matlab::MessageDeque deque;
  const matlab::Message msg_wrong_argin(matlab::MsgId::InvalidCommand,
    matlab::StringFunctionName(),
    "argin(1) must be valid command string");

  const auto argio_validators =  {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input, 1,
        (std::numeric_limits<int>::max)())),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output, 0,
        (std::numeric_limits<int>::max)()))
  };

  for (auto &validator : argio_validators) {
    if (validator->IsValid(input_count, input,
      output_count, output, deque) == false) {
      deque.Display(matlab::MsgLevel::Error);
      return;
    }
  }

  const auto command_validators = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char, 0)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::DimensionsCountArgumentValidator(0, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };
  bool valid_command = true;
  for (auto &validator : command_validators) {
    if (validator->IsValid(input_count, input, output_count,
      output, deque) == false) {
      valid_command = false;
      break;
    }
  }
  if (valid_command) {
    const std::string &command =
      utility::ToLowerCase(matlab::StringFromMxArray(input[0]));
    auto it = kCommandMap.find(command);
    if (it == kCommandMap.end() || it->second == nullptr) {
      msg_wrong_argin.Display(matlab::MsgLevel::Error);
      return;
    }
    NonpersistentData args(input_count, 1, input, output_count, 0, output);
    (*this.*it->second)(&args);
    return;
  }
  const auto handle_validators = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::DimensionsCountArgumentValidator(0, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()})),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Numeric, 0))
  };

  // TODO(rymut): documentation and tests
  bool valid_handle = true;
  for (auto &validator : handle_validators) {
    if (validator->IsValid(input_count, input,
      output_count, output, deque) == false) {
      valid_handle = false;
      break;
    }
  }
  if (valid_handle) {
    NonpersistentData args(input_count, 0, input, output_count, 0, output);
    this->CommandCall(&args);
    return;
  }

  msg_wrong_argin.Display(matlab::MsgLevel::Error);
}

// check implications of const mxArray
// **input not being a const mxArray * const * const input
/*! \brief Version command
 *  \summary Get version of cppmexclass mex
 *  [version-name [major [minor [patch [tweak]]]]] = mexcppclass('version')
 */

void Program::CommandVersion(NonpersistentDataInterface *args) {
  const int kArgOutVersion = args->ArgOutOffset();
  const int kArgOutMajor = kArgOutVersion + 1;
  const int kArgOutMinor = kArgOutMajor + 1;
  const int kArgOutPatch = kArgOutMinor + 1;
  const int kArgOutTweak = kArgOutPatch + 1;
  const int kArgOutMinCount = args->ArgOutOffset();
  const int kArgOutMaxCount = kArgOutTweak + 1;
  const int kArgInMinMaxCount = args->ArgInOffset();

  matlab::CountArgumentValidator validatorOut(matlab::Argument::Output,
    kArgOutMinCount, kArgOutMaxCount);
  matlab::CountArgumentValidator validatorIn(matlab::Argument::Input,
    kArgInMinMaxCount, kArgInMinMaxCount);

  matlab::MessageDeque deque;
  if (validatorOut.IsValid(args->ArgInCount(), args->ArgIn(),
    args->ArgOutCount(), args->ArgOut(), deque) == false ||
    validatorIn.IsValid(args->ArgInCount(), args->ArgIn(),
    args->ArgOutCount(), args->ArgOut(), deque) == false) {
    deque.Display(matlab::MsgLevel::Error,
      matlab::StringFunctionName() + ":version");
    return;
  }

  if (args->ArgOutCount() <= 0) {
    matlab::MxArrayUniquePtr version_name(
      mxCreateString(utility::VersionString().c_str()));
    matlab::SetAns(version_name.get());
  } else {
    switch (args->ArgOutCount() - args->ArgOutOffset()) {
      case 5: {
        auto tweak = utility::VersionTweak();
        if (tweak.has_value()) {
          args->ArgOut()[kArgOutTweak] = mxCreateDoubleScalar(tweak.value());
        } else {
          args->ArgOut()[kArgOutTweak] = mxCreateDoubleMatrix(0, 0, mxREAL);
        }
      }
      case 4: {
        auto patch = utility::VersionPatch();
        if (patch.has_value()) {
          args->ArgOut()[kArgOutPatch] = mxCreateDoubleScalar(patch.value());
        } else {
          args->ArgOut()[kArgOutPatch] = mxCreateDoubleMatrix(0, 0, mxREAL);
        }
      }
      case 3: {
        auto minor = utility::VersionMinor();
        if (minor.has_value()) {
          args->ArgOut()[kArgOutMinor] = mxCreateDoubleScalar(minor.value());
        } else {
          args->ArgOut()[kArgOutMinor] = mxCreateDoubleMatrix(0, 0, mxREAL);
        }
      }
      case 2: {
        auto major = utility::VersionMajor();
        if (major.has_value()) {
          args->ArgOut()[kArgOutMajor] = mxCreateDoubleScalar(major.value());
        } else {
          args->ArgOut()[kArgOutMajor] = mxCreateDoubleMatrix(0, 0, mxREAL);
        }
      }
      default: {
        args->ArgOut()[kArgOutVersion] =
          mxCreateString(utility::VersionString().c_str());
      }
    }
  }
}

/*! \brief command create
 *  \summary usage :
 *  [handle_id] = mexcppclass('create', 'name'[, module])
 */
void Program::CommandCreate(NonpersistentDataInterface *args) {
  const int kArgInHandleOrClass = args->ArgInOffset() + 0;
  const int kArgInMinCount = kArgInHandleOrClass + 1;
  const int kArgInMaxCount = (std::numeric_limits<int>::max)();
  const int kArgOutMinCount = args->ArgOutOffset();
  const int kArgOutHandle = args->ArgOutOffset();
  const int kArgOutMaxCount = kArgOutHandle + 1;
  matlab::MessageDeque deque;
  const auto required = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinCount, kArgInMaxCount)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutMinCount, kArgOutMaxCount)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        kArgInHandleOrClass)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::DimensionsCountArgumentValidator(kArgInHandleOrClass, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };
  for (auto &validator : required) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(),
      args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error);
      return;
    }
  }
  const std::string name =
    utility::ToLowerCase(
      matlab::StringFromMxArray(args->ArgIn()[kArgInHandleOrClass]));
    std::string module = "";
// \TODO(rymut): whats here?
  const auto optional = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ExistArgumentValidator(matlab::Argument::Input, 10)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        args->ArgInOffset() + 1)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::DimensionsCountArgumentValidator(args->ArgInOffset() + 1, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };

  bool valid_optional = true;
  for (auto &validator : optional) {
    if (validator.get() != nullptr &&
      validator->IsValid(args->ArgInCount(), args->ArgIn(),
        args->ArgOutCount(), args->ArgOut()) == false) {
      valid_optional = false;
    }
  }
  if (valid_optional == true) {
    module = utility::ToLowerCase(
      matlab::StringFromMxArray(args->ArgIn()[args->ArgInOffset() + 1]));
  }

  InstanceId id = kInstanceIdEmpty;
  {
    matlab::MessageDeque deque;

    InstancePointerInterface *pointer = nullptr;
    if (module.empty() == false) {
      // TODO(rymut): NOT USED ANYMORE
      pointer = this->module_mgr_.CreateInstance((name+":"+module).c_str());
    } else {
      pointer = this->module_mgr_.CreateInstance(name.c_str());
    }
    deque.Display(matlab::MsgLevel::Warning);
    if (pointer != nullptr && pointer->GetPointer() != nullptr) {
      id = this->instance_mgr_.Insert(pointer);
    }
    if (pointer != nullptr) {
      // USE this->module_mgr_.DeleteInstance(....);
      DeleteInstancePointer(pointer);
    }
  }
  this->module_mgr_.GarbageCollect();

  mxArray *mx_result = nullptr;
  if (id != 0) {
    mx_result = mxCreateDoubleScalar(id);
  } else {
    mx_result = mxCreateDoubleMatrix(0, 0, mxREAL);
  }
  if (args->ArgOutCount() <= 0) {
    matlab::MxArrayUniquePtr result(mx_result);
    matlab::SetAns(result.get());
  } else {
    args->ArgOut()[kArgOutHandle] = mx_result;
  }
}

/*! \brief Destroy command
 *  \summary usage
 *  [] = mexcppclass('destroy', handle_id)
 */
void Program::CommandDestroy(NonpersistentDataInterface *args) {
  const int kArgInHandle = args->ArgInOffset() + 1;
  const int kArgInMinMaxCount = kArgInHandle;
  const int kArgOutNone = args->ArgOutOffset() + 0;
  matlab::MessageDeque deque;
  const auto required = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinMaxCount, kArgInMinMaxCount)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutNone, kArgOutNone)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Scalar,
        kArgInHandle))
  };
  for (auto &validator : required) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(),
      args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error);
      return;
    }
  }
  const InstanceId id =
    static_cast<InstanceId>(mxGetScalar(args->ArgIn()[kArgInHandle]));
  this->instance_mgr_.Erase(id);
  // clear all not used pointers and modules
  this->module_mgr_.GarbageCollect();
}

void Program::CommandHandle(NonpersistentDataInterface *args) {
  // \TODO(rymut): not implement
  matlab::Message(matlab::MsgId::NotImplemented).Display(
    matlab::MsgLevel::Error);
}

void Program::CommandClear(NonpersistentDataInterface *args) {
  const int kArgInNone = args->ArgInOffset() + 0;
  const int kArgOutNone = args->ArgOutOffset() + 0;

  matlab::MessageDeque deque;
  const auto required = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInNone, kArgInNone)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutNone, kArgOutNone))
  };
  for (auto &validator : required) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(),
      args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error);
      return;
    }
  }
  this->instance_mgr_.Clear();
  this->module_mgr_.GarbageCollect();
}

/*! \brief Get methods of object
 *  \summary usage
 *  [methods] = mexcppclass('methods', handle);
 *  [methods] = mexcppclass('methods', 'type')
 */
void Program::CommandMethods(NonpersistentDataInterface *args) {
  const int kArgInHandleOrClass = args->ArgInOffset() + 0;
  const int kArgInMaxCount = args->ArgInOffset() + 1;
  const int kArgInMinCount = args->ArgInOffset()+ 1;
  const int kArgOutNone = args->ArgOutOffset();
  const int kArgOutMaxCount = args->ArgOutOffset() + 1;
  const int kArgOutMethods = args->ArgOutOffset() + 0;

  matlab::MessageDeque deque;
  const std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>
    common_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        kArgInMinCount,  kArgInMaxCount)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        kArgOutNone, kArgOutMaxCount))
  };
  const std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>
    methods_handle_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Numeric,
        kArgInHandleOrClass)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Scalar,
        kArgInHandleOrClass))
  };
  const std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>
    methods_class_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        kArgInHandleOrClass)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::DimensionsCountArgumentValidator(kArgInHandleOrClass, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };

  enum class Validator {
    kCommon,
    kMethodsHandle,
    kMethodsClass
  };

  const std::map<Validator,
    std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>>
    exec_path_validators = {
      { Validator::kCommon, common_validators },
      { Validator::kMethodsHandle, methods_handle_validators },
      { Validator::kMethodsClass, methods_class_validators }
  };

  std::map<Validator, matlab::MessageDeque> validator_deque = {
    { Validator::kCommon, matlab::MessageDeque() },
    { Validator::kMethodsHandle, matlab::MessageDeque() },
    { Validator::kMethodsClass, matlab::MessageDeque() }
  };

  for (auto &validators : exec_path_validators) {
    for (auto &validator : validators.second) {
      if (validator.get() != nullptr && validator->IsValid(
        args->ArgInCount(), args->ArgIn(), args->ArgOutCount(), args->ArgOut(),
        validator_deque[validators.first]) == false) {
        break;
      }
    }
  }

  std::vector<std::string> methods;
  if (validator_deque[Validator::kCommon].size() == 0) {
    if (validator_deque[Validator::kMethodsHandle].size() == 0) {
      InstancePointerInterface *pointer = this->instance_mgr_.Pointer(
        static_cast<InstanceId>(
          mxGetScalar(args->ArgIn()[kArgInHandleOrClass])));
      if (pointer == nullptr) {
        matlab::Message(matlab::MsgId::InvalidHandle, "program",
          matlab::ArgName(matlab::Argument::Input,
            kArgInHandleOrClass)).Display(matlab::MsgLevel::Error);
        return;
      }
      methods.resize(pointer->GetPointer()->MethodCount());
      for (size_t method = 0; method < methods.size(); ++method) {
        const size_t lenght = pointer->GetPointer()->Method(method) + 1;
        std::unique_ptr<char[]> name(new char[lenght]);
        pointer->GetPointer()->Method(method, lenght, name.get());
        methods[method] = std::string(name.get());
      }
      DeleteInstancePointer(pointer);
    } else if (validator_deque[Validator::kMethodsClass].size() == 0) {
      matlab::MessageDeque deque;
      InstancePointerInterface *pointer = this->module_mgr_.CreateInstance(
        matlab::StringFromMxArray(args->ArgIn()[kArgInHandleOrClass]).c_str());
      deque.Display(matlab::MsgLevel::Warning);
      if (pointer == nullptr) {
        matlab::Message(matlab::MsgId::InvalidClassId, "program",
          matlab::ArgName(matlab::Argument::Input,
            kArgInHandleOrClass)).Display(matlab::MsgLevel::Error);
        return;
      }
      methods.resize(pointer->GetPointer()->MethodCount());
      for (size_t method = 0; method < methods.size(); ++method) {
        const size_t lenght = pointer->GetPointer()->Method(method) + 1;
        std::unique_ptr<char[]> name(new char[lenght]);
        pointer->GetPointer()->Method(method, lenght, name.get());
        methods[method] = std::string(name.get());
      }
      DeleteInstancePointer(pointer);
    } else {
      validator_deque[Validator::kMethodsClass].Display(
        matlab::MsgLevel::Error);
      validator_deque[Validator::kMethodsHandle].Display(
        matlab::MsgLevel::Error);
      matlab::Message(matlab::MsgId::InvalidArguments, "methods",
        "test").Display(matlab::MsgLevel::Error);
      return;
    }
    // in case of 'class' variant
    this->module_mgr_.GarbageCollect();
    mxArray *matrix = matlab::CellFromVector(methods, matlab::ArrayFromString);
    if (matrix == nullptr) {
      matlab::Message(matlab::MsgId::MemoryAllocationFailure,
        "methods").Display(matlab::MsgLevel::Error);
      return;
    }
    if (args->ArgOutCount() <= kArgOutMethods) {
      matlab::MxArrayUniquePtr result(matrix);
      matlab::SetAns(result.get());
    } else {
      args->ArgOut()[kArgOutMethods] = matrix;
    }
  } else {
    validator_deque[Validator::kCommon].Display(matlab::MsgLevel::Error);
  }
}


/*! \brief Get class
 *  \summary usage:
 *  [class] = mexcppclass('class', handle)
 */
// Program::CommandClass

/*! \brief register module
 *  \summary usage
 *  [status] = mexcppclass('module', 'name')
 */
// Program::CommandModule

/*! \brief Get classes
 *  \summary usage:
 * [classes] = mexcppclass('classes');
 */

/*! \brief Create instance of class
 *  \summary usage:
 *  [handle] = mexcppclass('create', 'class');
 */
void Program::CommandCall(NonpersistentDataInterface *args) {
  if (args == nullptr) {
    // \TODO(brymut): massage
    return;
  }
  matlab::MessageDeque deque;
  const auto required = {
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset() + 2, (std::numeric_limits<int>::max)())),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        args->ArgOutOffset() + 0, (std::numeric_limits<int>::max)())),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Scalar,
        args->ArgInOffset() + 0)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Numeric,
        args->ArgInOffset() + 0)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Char,
        args->ArgInOffset() + 1)),
    matlab::ArgumentValidatorInterfaceUniquePtr(
      new matlab::DimensionsCountArgumentValidator(args->ArgInOffset() + 1, 2,
        {1LL, 1LL, 1LL, (std::numeric_limits<mwSize>::max)()}))
  };
  for (auto &validator : required) {
    if (validator.get() != nullptr && validator->IsValid(args->ArgInCount(),
      args->ArgIn(), args->ArgOutCount(), args->ArgOut(), deque) == false) {
      deque.Display(matlab::MsgLevel::Error);
      return;
    }
  }
  const InstanceId id =
    static_cast<InstanceId>(mxGetScalar(args->ArgIn()[args->ArgInOffset()+0]));
  const std::string method =
    utility::ToLowerCase(matlab::StringFromMxArray(
      args->ArgIn()[args->ArgInOffset() + 1]));

  mxArray *mx_result = mx_result = mxCreateDoubleScalar(id);
  if (args->ArgOutCount() <= 0) {
    matlab::MxArrayUniquePtr result(mx_result);
    matlab::SetAns(result.get());
  } else {
    args->ArgOut()[args->ArgOutOffset()] = mx_result;
  }

  InstancePointerInterface *pointer = this->instance_mgr_.Pointer(id);
  if (pointer == nullptr) {
    matlab::Message(matlab::MsgId::InvalidHandle, "mexFunction",
      "argin(2)").Display(matlab::MsgLevel::Error);
    return;
  }
  // TODO(brymut): before GetPointer check if raw pointer is not null
  std::vector<std::string> methods(pointer->GetPointer()->MethodCount());
  for (size_t method = 0; method < methods.size(); ++method) {
    const size_t lenght = pointer->GetPointer()->Method(method) + 1;
    std::unique_ptr<char[]> name(new char[lenght]);
    pointer->GetPointer()->Method(method, lenght, name.get());
    methods[method] = std::string(name.get());
  }
  if (methods.end() == find(methods.begin(), methods.end(), method)) {
    matlab::Message(matlab::MsgId::Custom, "mexFunction",
      "argin(3) is valid method name").Display(matlab::MsgLevel::Error);
    DeleteInstancePointer(pointer);
    return;
  }

  PersistentData mgr(&this->instance_mgr_, &this->module_mgr_);
  // setting ans
  pointer->GetPointer()->Call(method.c_str(), args, &mgr);
  DeleteInstancePointer(pointer);
}

}  // namespace mexcppclass
