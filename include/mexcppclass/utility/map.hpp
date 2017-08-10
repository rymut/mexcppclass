/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_UTILITY_MAP_HPP_
#define MEXCPPCLASS_UTILITY_MAP_HPP_

#include <optional>
#include <map>
#include <string>

namespace mexcppclass {
namespace utility {

/*! \brief Function returns optional map value
 */
template <typename K, typename V> typename std::optional<V> MapValue(
  typename const std::map<K, V> &map,
  typename const K &key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return std::optional<V>();
  }
  return std::optional<V>(it->second);
}

}  // namespace utility
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_UTILITY_MAP_HPP_
