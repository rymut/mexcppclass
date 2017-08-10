/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_INSTANCEID_HPP_
#define MEXCPPCLASS_INSTANCEID_HPP_

#pragma once

namespace mexcppclass {

/*! \brief Handle ID type
 */
typedef unsigned int InstanceId;

/*! \brief Empty handle
 */
constexpr InstanceId kInstanceIdEmpty = 0;

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_INSTANCEID_HPP_
