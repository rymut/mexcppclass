/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_NONPERSISTENTDATAINTERFACE_HPP_
#define MEXCPPCLASS_NONPERSISTENTDATAINTERFACE_HPP_

#include <mex.h>
#include "mexcppclass/export.hpp"

namespace mexcppclass {

class MEXCPPCLASS_EXPORT NonpersistentDataInterface {
 protected:
  virtual ~NonpersistentDataInterface() = 0;
 public:
  virtual void SetArgOut(const int &position, mxArray *value) = 0;
  virtual const mxArray * const * const ArgOut() const = 0;
  virtual mxArray **ArgOut() = 0;
  virtual const mxArray * const * const ArgIn() const = 0;
  virtual const int &ArgInCount() const = 0;
  virtual const int &ArgOutCount() const = 0;

  virtual const int &ArgInOffset() const = 0;
  virtual const int &ArgOutOffset() const = 0;
};

inline NonpersistentDataInterface::~NonpersistentDataInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_NONPERSISTENTDATAINTERFACE_HPP_
