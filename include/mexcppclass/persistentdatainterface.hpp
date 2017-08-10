/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_PERSISTENTDATAINTERFACE_HPP_
#define MEXCPPCLASS_PERSISTENTDATAINTERFACE_HPP_

#include "mexcppclass/export.hpp"
#include "mexcppclass/instancemanagerinterface.hpp"
#include "mexcppclass/modulemanagerinterface.hpp"

namespace mexcppclass {

class MEXCPPCLASS_EXPORT PersistentDataInterface {
 public:
  virtual InstanceManagerInterface *InstanceMgr() const = 0;
  virtual ModuleManagerInterface *ModuleMgr() const = 0;

 protected:
  /*! \brief Pure virtual destructor
   */
  virtual ~PersistentDataInterface() = 0;
};

inline PersistentDataInterface::~PersistentDataInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_PERSISTENTDATAINTERFACE_HPP_
