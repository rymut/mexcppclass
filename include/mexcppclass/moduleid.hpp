/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MODULEID_HPP_
#define MEXCPPCLASS_MODULEID_HPP_

namespace mexcppclass {

typedef size_t ModuleId;

constexpr ModuleId kModuleIdEmpty = 0;

constexpr ModuleId kModuleIdMexFunction = 1;

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MODULEID_HPP_
