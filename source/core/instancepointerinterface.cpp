/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/instancepointerinterface.hpp"
#include "mexcppclass/export.hpp"
#include "core/instancepointer.hpp"

extern "C" {

MEXCPPCLASS_EXPORT void DeleteInstancePointerInterface(
  mexcppclass::InstancePointerInterface *obj) {
  mexcppclass::InstancePointer *pointer =
    dynamic_cast<mexcppclass::InstancePointer*>(obj);
  if (pointer != nullptr) {
    delete pointer;
  } else {
    // \TODO (rymut): log message can result in some problems later
  }
}

}  // extern "C"
