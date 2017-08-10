/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_PERSISTENTDATA_HPP_
#define MEXCPPCLASS_CORE_PERSISTENTDATA_HPP_

#include "mexcppclass/persistentdatainterface.hpp"
#include "modulemanager.hpp"
#include "instancemanager.hpp"

namespace mexcppclass {

/*! \brief PersistentData between the calls
 */
class PersistentData : public PersistentDataInterface {
 public:
  PersistentData(InstanceManager *instance_mgr, ModuleManager *module_mgr);

  ~PersistentData();

  virtual InstanceManagerInterface *InstanceMgr() const;

  virtual ModuleManagerInterface *ModuleMgr() const;

 private:
  InstanceManager *instance_mgr_;
  ModuleManager *module_mgr_;
};

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_PERSISTENTDATA_HPP_
