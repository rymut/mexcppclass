/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_TEST_UNIT_TESTMODULE_HPP_
#define MEXCPPCLASS_TEST_UNIT_TESTMODULE_HPP_

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "mexcppclass/moduleinterface.hpp"
#include "test/unit/testmodulemethod.hpp"
#include "test/unit/test.hpp"

namespace mexcppclass {
namespace test {
namespace unit {

/*! \brief Example matlab module
 */
class TestModule : public ModuleInterface {
 public:
  TestModule();
  ~TestModule();

  virtual size_t MethodCount() const;
  virtual size_t Method(const size_t &method, const size_t &lenght,
    char *name) const;
  virtual void Call(const char *method, NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

  virtual const char *Name() const;

  void MethodCreate(NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

  void MethodDestroy(NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

  void MethodSetValue(NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

  void MethodGetValue(NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

  void MethodIsEqual(NonpersistentDataInterface *args,
    PersistentDataInterface *mgr);

 private:
  /*! \brief map actions with text
  */
  std::map<std::string, TestModuleMethod> string_method_map_ = {
    { "create", TestModuleMethod::kCreate },
    { "destroy", TestModuleMethod::kDestroy },
    { "setvalue", TestModuleMethod::kSetValue },
    { "getvalue", TestModuleMethod::kGetValue },
    { "isequal", TestModuleMethod::kIsEqual }
  };
  std::map<TestModuleMethod,
    void(TestModule::*)(NonpersistentDataInterface*, PersistentDataInterface*)>
    method_member_map_ = {
    { TestModuleMethod::kCreate, &TestModule::MethodCreate },
    { TestModuleMethod::kDestroy, &TestModule::MethodDestroy },
    { TestModuleMethod::kSetValue, &TestModule::MethodSetValue },
    { TestModuleMethod::kGetValue, &TestModule::MethodGetValue },
    { TestModuleMethod::kIsEqual, &TestModule::MethodIsEqual }
  };
  std::unique_ptr<Test> object_;
};

}  // namespace unit
}  // namespace test
}  // namespace mexcppclass

extern "C" {
  __declspec(dllexport)
    mexcppclass::ModuleInterface *CreateTestModuleInterface();
  __declspec(dllexport)
    void DestroyTestModuleInterface(mexcppclass::ModuleInterface *obj);

  __declspec(dllexport)
    void DestroyTestInternalModuleInterface(mexcppclass::ModuleInterface *obj);
}

#endif  // MEXCPPCLASS_TEST_UNIT_TESTMODULE_HPP_
