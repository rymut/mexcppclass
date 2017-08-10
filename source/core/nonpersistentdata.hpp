/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_NONPERSISTENTDATA_HPP_
#define MEXCPPCLASS_CORE_NONPERSISTENTDATA_HPP_

#include "mexcppclass/nonpersistentdatainterface.hpp"

namespace mexcppclass {

class NonpersistentData : public NonpersistentDataInterface {
 public:
  NonpersistentData(const int &arg_in_count, const int &arg_in_offset,
    const mxArray * const * const arg_in,
    const int &arg_out_count, const int &arg_out_offset, mxArray **arg_out);

  ~NonpersistentData();

  void SetArgOut(const int &position, mxArray *value);

  const mxArray * const * const ArgOut() const;

  mxArray **ArgOut();

  const mxArray * const * const ArgIn() const;

  const int &ArgInCount() const;

  const int &ArgOutCount() const;

  const int &ArgInOffset() const;

  const int &ArgOutOffset() const;

 private:
  const mxArray * const * const arg_in_;
  mxArray **arg_out_;
  const int arg_in_count_;
  const int arg_out_count_;
  int arg_in_offset_;
  int arg_out_offset_;
};

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_NONPERSISTENTDATA_HPP_
