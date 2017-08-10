/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_INSTANCEMANAGERINTERFACE_HPP_
#define MEXCPPCLASS_INSTANCEMANAGERINTERFACE_HPP_

#include "mexcppclass/export.hpp"
#include "mexcppclass/instanceid.hpp"
#include "mexcppclass/instancepointerinterface.hpp"

namespace mexcppclass {

class MEXCPPCLASS_EXPORT InstanceManagerInterface {
 protected:
  /*! \brief Pure virtual destructor
   */
  virtual ~InstanceManagerInterface() = 0;

 public:
  /*! \brief Remove instance
   */
  virtual void Erase(const InstanceId &id) = 0;

  /*! \brief Insert instance
   */
  virtual InstanceId Insert(const InstancePointerInterface *pointer) = 0;

  /*! \brief Get instance
   */
  virtual InstancePointerInterface *Pointer(const InstanceId &id) const = 0;

  /*! \brief Clear
   */
  virtual void Clear() = 0;
};

inline InstanceManagerInterface::~InstanceManagerInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_INSTANCEMANAGERINTERFACE_HPP_
