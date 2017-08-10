/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

// definition headers
#include "mexcppclass/utility/version.hpp"

// C++ headers
#include <optional>
#include <sstream>
#include <string>

// project includes
#include "utility/version.h"

namespace mexcppclass {
namespace utility {

std::string VersionString() {
  std::string version = "";
  const std::string tweak(VERSION_TWEAK);
  const std::string patch(VERSION_PATCH);
  const std::string minor(VERSION_MINOR);
  const std::string major(VERSION_MAJOR);
  if (tweak.empty() == false) {
    version = "." + tweak;
  }
  if (patch.empty() == false) {
    version = "." + patch + version;
  } else if (version.empty() == false) {
    version = ".0" + version;
  }
  if (minor.empty() == false) {
    version = "." + minor + version;
  } else if (version.empty() == false) {
    version = ".0" + version;
  }
  if (major.empty() == false) {
    version = major + version;
  } else if (version.empty() == false) {
    version = "0." + version;
  } else {
    version = "0";
  }
  return version;
}

static std::optional<unsigned int> VersionNumber(const std::string &value) {
  std::stringstream stream;
  stream << value;
  std::optional<unsigned int> number;
  unsigned int numeric_value = 0;
  stream >> numeric_value;
  if (stream.fail() != true) {
    number = numeric_value;
  }
  return number;
}
std::optional<unsigned int> VersionMajor() {
  return VersionNumber(VERSION_MAJOR);
}

std::optional<unsigned int> VersionMinor() {
  return VersionNumber(VERSION_MINOR);
}

std::optional<unsigned int> VersionPatch() {
  return VersionNumber(VERSION_PATCH);
}

std::optional<unsigned int> VersionTweak() {
  return VersionNumber(VERSION_TWEAK);
}

}  // namespace utility
}  // namespace mexcppclass
