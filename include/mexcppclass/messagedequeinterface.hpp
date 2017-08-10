/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MESSAGEDEQUEINTERFACE_HPP_
#define MEXCPPCLASS_MESSAGEDEQUEINTERFACE_HPP_

#include "mexcppclass/export.hpp"

namespace mexcppclass {

class MEXCPPCLASS_EXPORT MessageDequeInterface {
 protected:
  /*! \brief Pure virtual destructor
   */
  virtual ~MessageDequeInterface() = 0;

 public:
};

inline MessageDequeInterface::~MessageDequeInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MESSAGEDEQUEINTERFACE_HPP_
