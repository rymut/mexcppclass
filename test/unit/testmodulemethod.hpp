/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_TEST_UNIT_TESTMODULEMETHOD_HPP_
#define MEXCPPCLASS_TEST_UNIT_TESTMODULEMETHOD_HPP_

namespace mexcppclass {
namespace test {
namespace unit {

/*! \brief Example action
 */
enum class TestModuleMethod {
  kCreate,  /*!< create instance */
  kDestroy,  /*!< destroy instance */
  kSetValue,  /*!< set value */
  kGetValue, /*!< get value */
  kIsEqual,  /*!< equal operator */
};

}  // namespace unit
}  // namespace test
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_TEST_UNIT_TESTMODULEMETHOD_HPP_
