/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_UTILITY_VERSION_HPP_
#define MEXCPPCLASS_UTILITY_VERSION_HPP_

#include <string>
// CPPLINT does not like including <optional>
#include "optional"

namespace mexcppclass {
namespace utility {

std::string VersionString();

std::optional<unsigned int> VersionMajor();

std::optional<unsigned int> VersionMinor();

std::optional<unsigned int> VersionPatch();

std::optional<unsigned int> VersionTweak();

}  // namespace utility
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_UTILITY_VERSION_HPP_
