/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/persistentdata.hpp"

namespace mexcppclass {

PersistentData::PersistentData(InstanceManager *instance_mgr,
  ModuleManager *module_mgr) : instance_mgr_(instance_mgr),
  module_mgr_(module_mgr) {
}

PersistentData::~PersistentData() {
}

InstanceManagerInterface *PersistentData::InstanceMgr() const {
  return this->instance_mgr_;
}
ModuleManagerInterface *PersistentData::ModuleMgr() const {
  return this->module_mgr_;
}

}  // namespace mexcppclass
