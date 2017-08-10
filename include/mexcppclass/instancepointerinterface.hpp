/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_INSTANCEPOINTERINTERFACE_HPP_
#define MEXCPPCLASS_INSTANCEPOINTERINTERFACE_HPP_

#include "mexcppclass/export.hpp"

namespace mexcppclass {

class ModuleInterface;

class MEXCPPCLASS_EXPORT InstancePointerInterface {
 public:
  /*! \brief Raw pointer type definition
   */
  typedef ModuleInterface *Pointer;

  /*! \biref Get raw pointer
   *  \return raw pointer
   */
  virtual Pointer GetPointer() const = 0;

 protected:
  /*! \brief Pure virtual destructor
   */
  virtual ~InstancePointerInterface() = 0;
};

inline InstancePointerInterface::~InstancePointerInterface() {
}

}  // namespace mexcppclass

extern "C" {
MEXCPPCLASS_EXPORT void DeleteInstancePointerInterface(
  mexcppclass::InstancePointerInterface *obj);
}  // extern "C"

#endif  // MEXCPPCLASS_INSTANCEPOINTERINTERFACE_HPP_
