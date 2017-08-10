/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "test/unit/testmodule.hpp"

#include <map>
#include <string>
#include <vector>

#include "mexcppclass/matlab/msgid.hpp"
#include "mexcppclass/utility/map.hpp"
#include "mexcppclass/pointermodule.hpp"
#include "mexcppclass/matlab/messagedeque.hpp"
#include "mexcppclass/matlab/argumentvalidatorinterfacememory.hpp"
#include "mexcppclass/matlab/countargumentvalidator.hpp"
#include "mexcppclass/matlab/classidargumentvalidator.hpp"


extern "C" {
__declspec(dllexport)
  mexcppclass::ModuleInterface *CreateTestModuleInterface() {
  return new mexcppclass::test::unit::TestModule();
}
__declspec(dllexport)
  void DestroyTestModuleInterface(mexcppclass::ModuleInterface *obj) {
  mexcppclass::test::unit::TestModule *test_module_obj =
    dynamic_cast<mexcppclass::test::unit::TestModule*>(obj);
  if (test_module_obj) {
    delete test_module_obj;
  } else {
    // mexPrintf("cannot convert to test module\n");
  }
}
__declspec(dllexport)
  void DestroyTestInternalModuleInterface(mexcppclass::ModuleInterface *obj) {
    mexcppclass::DestroyPointerModuleInterface<
      mexcppclass::test::unit::TestInternal>(obj);
}
}  // extern "C"

namespace mexcppclass {
namespace test {
namespace unit {

TestModule::TestModule() {
}

TestModule::~TestModule() {
}

size_t TestModule::MethodCount() const {
  return this->string_method_map_.size();
}

size_t TestModule::Method(const size_t &method,
  const size_t &length, char * name) const {
  const std::string &method_name = (method < this->string_method_map_.size())?
    std::next(this->string_method_map_.begin(), method)->first:"";
  if (name == nullptr || length == 0) {
    return method_name.length();
  }
  name[method_name.copy(name, length-1)] = '\0';
  return method_name.length();
}

void TestModule::Call(const char *method, NonpersistentDataInterface *args,
  PersistentDataInterface *mgr) {
  if (method == nullptr) {
    return;
  }

  auto method_enum = utility::MapValue(this->string_method_map_,
    std::string(method));
  if (!method_enum) {
    matlab::Message(matlab::MsgId::InvalidArgument, "mexargument",
      "mex argument errorr").Display(matlab::MsgLevel::Error);
  }

  if (method_enum == TestModuleMethod::kCreate && this->object_.get() !=
    nullptr) {
    matlab::Message(matlab::MsgId::Custom, "object already created").Display(
      matlab::MsgLevel::Error);
    return;
  } else if (this->object_.get() == nullptr) {
    if (method_enum == TestModuleMethod::kDestroy) {
      matlab::Message(matlab::MsgId::Custom, "testmodule",
        "object already destroyed").Display(matlab::MsgLevel::Error);
      return;
    } else if (method_enum != TestModuleMethod::kCreate) {
      matlab::Message(matlab::MsgId::Custom, "testmodule",
        "cannot run method on non existing object").Display(
          matlab::MsgLevel::Error);
      return;
    }
  }

  auto member = utility::MapValue(
    this->method_member_map_, method_enum.value());
  if (!member) {
    matlab::Message(matlab::MsgId::NotImplemented).Display(
      matlab::MsgLevel::Error);
    return;
  }

  (this->*member.value())(args, mgr);
}

const char *TestModule::Name() const {
  return "test";
}


void TestModule::MethodCreate(NonpersistentDataInterface *args,
  PersistentDataInterface *mgr) {
  if (args == nullptr) {
    // \TODO(rymut): error message
    return;
  }
  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>
    common_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        args->ArgOutOffset() + 0, args->ArgOutOffset() + 0))
  };

  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>
    default_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset() + 0, args->ArgInOffset() + 0))
  };

  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr> value_validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset() + 1, args->ArgInOffset() + 1)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Numeric,
        args->ArgInOffset())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Scalar,
        args->ArgInOffset()))
  };

  enum class Validator {
    kCommon,
    kDefault,
    kValue
  };
// todo copy constructor
  const std::map<Validator,
    std::vector<matlab::ArgumentValidatorInterfaceSharedPtr>> all_validators = {
      { Validator::kCommon, common_validators },
      { Validator::kDefault, default_validators },
      { Validator::kValue, value_validators }
  };

  std::map<Validator, matlab::MessageDeque> validator_deque = {
    { Validator::kCommon, matlab::MessageDeque() },
    { Validator::kDefault, matlab::MessageDeque() },
    { Validator::kValue, matlab::MessageDeque() }
  };

  for (auto &validators : all_validators) {
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
    if (validator_deque[Validator::kDefault].size() == 0) {
      this->object_.reset(new Test());
    } else if (validator_deque[Validator::kValue].size() == 0) {
      double value = static_cast<double>(mxGetScalar(
        args->ArgIn()[args->ArgInOffset()]));
      this->object_.reset(new Test(value));
    } else {
      validator_deque[Validator::kDefault].Display(matlab::MsgLevel::Warning);
      mexPrintf("\n\n");
      validator_deque[Validator::kCommon].Display(matlab::MsgLevel::Warning);
      mexPrintf("\n\n");
      matlab::Message(matlab::MsgId::InvalidArguments, "methods",
        "test").Display(matlab::MsgLevel::Error);
      return;
    }
  } else {
    validator_deque[Validator::kCommon].Display(matlab::MsgLevel::Warning);
  }
}

void TestModule::MethodDestroy(NonpersistentDataInterface *args,
  PersistentDataInterface *mgr) {
  if (args == nullptr) {
    // TODO(rymut): error message
    return;
  }
  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr> validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset(), args->ArgInOffset())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        args->ArgOutOffset(), args->ArgOutOffset())),
  };
  matlab::MessageDeque deque;

  for (auto &validator : validators) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(), args->ArgOutCount(), args->ArgOut(),
      deque) == false) {
      deque.Display(matlab::MsgLevel::Warning);
      return;
    }
  }

  this->object_.reset(nullptr);
}

void TestModule::MethodSetValue(NonpersistentDataInterface *args,
  PersistentDataInterface *mgr) {
  if (args == nullptr) {
    // TODO(rymut): error message
    return;
  }
  if (this->object_.get() == nullptr) {
    matlab::Message(matlab::MsgId::Custom, "object does not exist").Display(
      matlab::MsgLevel::Error);
    return;
  }

  matlab::MessageDeque deque;
  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr> validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset() + 1, args->ArgInOffset() + 1)),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Scalar,
        args->ArgInOffset())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::ClassIdArgumentValidator(matlab::ClassId::Numeric,
        args->ArgInOffset())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        args->ArgOutOffset(), args->ArgOutOffset())),
  };

  for (auto &validator : validators) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(), args->ArgOutCount(), args->ArgOut(),
      deque) == false) {
      deque.Display(matlab::MsgLevel::Warning);
      return;
    }
  }

  const double value = static_cast<double>(mxGetScalar(
    args->ArgIn()[args->ArgInOffset()]));
  this->object_->SetValue(value);
}

void TestModule::MethodGetValue(NonpersistentDataInterface *args,
  PersistentDataInterface *mgr) {
  if (args == nullptr) {
    // TODO(rymut): error message
    return;
  }
  if (this->object_.get() == nullptr) {
    matlab::Message(matlab::MsgId::Custom, "object does not exist").Display(
      matlab::MsgLevel::Error);
    return;
  }

  matlab::MessageDeque deque;
  std::vector<matlab::ArgumentValidatorInterfaceSharedPtr> validators = {
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Input,
        args->ArgInOffset(), args->ArgInOffset())),
    matlab::ArgumentValidatorInterfaceSharedPtr(
      new matlab::CountArgumentValidator(matlab::Argument::Output,
        args->ArgOutOffset() + 1, args->ArgOutOffset() + 1)),
  };

  for (auto &validator : validators) {
    if (validator.get() != nullptr && validator->IsValid(
      args->ArgInCount(), args->ArgIn(), args->ArgOutCount(), args->ArgOut(),
      deque) == false) {
      deque.Display(matlab::MsgLevel::Warning);
      return;
    }
  }

  const double value = this->object_->GetValue();
  args->ArgOut()[args->ArgOutOffset()] = mxCreateDoubleScalar(value);
}

void TestModule::MethodIsEqual(NonpersistentDataInterface *args,
  PersistentDataInterface *data) {
  if (args == nullptr || data == nullptr) {
    // TODO(rymut): error message
    return;
  }
  if (data->ModuleMgr() == nullptr || data->InstanceMgr() == nullptr) {
    // TODO(rymut): error module mgr is null
    return;
  }
  // test module creation
  auto ptr = new TestInternal();
  PointerModule<TestInternal> *mod_ptr = new PointerModule<TestInternal>(
    "testinternal", ptr);
  // pointer module will automicaly destroy ptr on failure
  InstancePointerInterface *pointer = data->ModuleMgr()->Manage(mod_ptr,
    &DestroyTestInternalModuleInterface, this);
  if (pointer == nullptr) {
    // TODO(rymut): message cannot track the pointer
    delete mod_ptr;
  } else {
    auto id = data->InstanceMgr()->Insert(pointer);
    DeleteInstancePointerInterface(pointer);
  }
}

}  // namespace unit
}  // namespace test
}  // namespace mexcppclass
