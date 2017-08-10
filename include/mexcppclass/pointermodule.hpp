/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_POINTERMODULE_HPP_
#define MEXCPPCLASS_POINTERMODULE_HPP_

#include <mex.h>
#include <memory>
#include <vector>
#include <string>
#include "mexcppclass/persistentdatainterface.hpp"
#include "mexcppclass/nonpersistentdatainterface.hpp"
#include "mexcppclass/moduleinterface.hpp"

namespace mexcppclass {

template <typename T>
class PointerModule : public ModuleInterface {
 public:
  PointerModule(const std::string &name, T *object) : name_(name) {
    this->object_.reset(object);
  }

  virtual ~PointerModule() {
  }

  virtual size_t MethodCount() const {
    return 0;
  }

  virtual size_t Method(const size_t &method, const size_t &length,
    char *name) const {
    return 0;
  }

  virtual void Call(const char *method, NonpersistentDataInterface *args,
    PersistentDataInterface *mgr) {
    // no method are suported
  }
  virtual const char * Name() const {
    return this->name_.c_str();
  }

  T *Pointer() const {
    return this->object_.get();
  }

 private:
  typename std::unique_ptr<T> object_;
  const std::string &name_;
};

template <typename T>
void DestroyPointerModuleInterface(ModuleInterface *object) {
  typename PointerModule<T> *pointer =
    dynamic_cast<typename PointerModule<T>*>(object);
  if (pointer != nullptr) {
    delete pointer;
  }
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_POINTERMODULE_HPP_
