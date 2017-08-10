/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

/*! \file mexcppclass/action.hpp
 *  \brief Declaration of ModuleInterface cration function
 *
 *  \author Boguslaw Rymut (rymut)
 *  \bugs No known bugs.
 */

#ifndef MEXCPPCLASS_MODULEINTERFACEFUNCTIONPOINTER_HPP_
#define MEXCPPCLASS_MODULEINTERFACEFUNCTIONPOINTER_HPP_

namespace mexcppclass {

// forward declaration of type
class ModuleInterface;

/*! \brief Module interface function pointer type
 */
typedef ModuleInterface *(*ModuleInterfaceCreateFuncPtr)(void);

typedef void (*ModuleInterfaceDestroyFuncPtr)(ModuleInterface *);

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MODULEINTERFACEFUNCTIONPOINTER_HPP_
