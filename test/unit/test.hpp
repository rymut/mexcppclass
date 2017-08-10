/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_TEST_UNIT_TEST_HPP_
#define MEXCPPCLASS_TEST_UNIT_TEST_HPP_

namespace mexcppclass {
namespace test {
namespace unit {

class Test {
 public:
  Test();
  explicit Test(const double &value);
  Test(const Test &rhs);
  ~Test();
  void SetValue(const double &value);
  const double &GetValue() const;
  bool IsEqual(const Test &object) const;
  bool IsEqual(const double &value) const;
  Test &operator=(const Test &rhs);

 private:
  double value_;
};

class TestInternal {
 public:
  TestInternal();
  ~TestInternal();
};

}  // namespace unit
}  // namespace test
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_TEST_UNIT_TEST_HPP_
