/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MODULEMANAGERINTERFACE_HPP_
#define MEXCPPCLASS_MODULEMANAGERINTERFACE_HPP_

#include "mexcppclass/export.hpp"
#include "mexcppclass/instanceid.hpp"
#include "mexcppclass/moduleinterfacefunctionpointer.hpp"

namespace mexcppclass {

class MEXCPPCLASS_EXPORT ModuleManagerInterface {
 public:
  virtual ~ModuleManagerInterface() = 0;

  virtual InstancePointerInterface *CreateInstance(
    const char *synonom_or_fullname) = 0;

  virtual InstancePointerInterface *Manage(ModuleInterface *module,
    ModuleInterfaceDestroyFuncPtr destroy_func_ptr,
    ModuleInterface *parent) = 0;

  virtual void GarbageCollect() = 0;
};

inline ModuleManagerInterface::~ModuleManagerInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MODULEMANAGERINTERFACE_HPP_
