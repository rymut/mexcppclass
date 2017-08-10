/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_CLASSID_HPP_
#define MEXCPPCLASS_MATLAB_CLASSID_HPP_

namespace mexcppclass {
namespace matlab {

enum class ClassId {
  Single,
  Double,
  Char,
  String,
  Logical,
  Int8,
  Int16,
  Int32,
  Int64,
  Uint8,
  Uint16,
  Uint32,
  Uint64,
  Numeric,
  Scalar,
  Struct,
  Cell,
  Table
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_CLASSID_HPP_
