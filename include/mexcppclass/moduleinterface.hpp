/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

/*! \file mexcppclass/action.hpp
 *  \brief Declaration of ModuleInterface class
 *
 *  \author Boguslaw Rymut (rymut)
 *  \bugs No known bugs.
 */

#ifndef MEXCPPCLASS_MODULEINTERFACE_HPP_
#define MEXCPPCLASS_MODULEINTERFACE_HPP_

#include <mex.h>

// \TODO (rymut): remove string and vector for library interface usage
#include <string>
#include <vector>

#include "mexcppclass/nonpersistentdatainterface.hpp"
#include "mexcppclass/persistentdatainterface.hpp"
#include "mexcppclass/export.hpp"

namespace mexcppclass {

/*! \brief Module interface
 */
class MEXCPPCLASS_EXPORT ModuleInterface {
 public:
  /*! \brief Pure virtual destructor
   */
  virtual ~ModuleInterface() = 0;

  /*! \brief Get methods number
   *  \return Return number of methods
   */
  virtual size_t MethodCount() const = 0;

  /*! \brief Get method name and lenght
   *  \param method - method id
   *  \param lenght - lenght of passed memory
   *  \param name - memory to store method name
   *  \param method name
   */
  virtual size_t Method(const size_t &method, const size_t &length = 0,
    char *name = nullptr) const = 0;

  /*! \brief Call method
   *  \param method - method name
   *  \param nlhs - numer of left hand side arguments
   *  \param plhs - pointer to left hand side arguments
   *  \param olhs - offset of left hand size arguments
   *  \param nrhs - numer of right hand side arguments
   *  \param prhs - pointer to right hand side arguments
   *  \param olhs - offset of right hand side arguments
   */
  virtual void Call(const char *method,
    NonpersistentDataInterface *args, PersistentDataInterface *mgr) = 0;

  /*! \brief Get name of the object
   *  \return name of the object
   */
  virtual const char *Name() const = 0;
};

inline ModuleInterface::~ModuleInterface() {
}

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MODULEINTERFACE_HPP_
