/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_INSTANCEMANAGER_HPP_
#define MEXCPPCLASS_CORE_INSTANCEMANAGER_HPP_
// C++ standard headrs
#include <map>
#include <string>
#include <utility>
#include <vector>
// project headers
#include "core/instancepointer.hpp"
#include "mexcppclass/instanceid.hpp"
#include "mexcppclass/instancemanagerinterface.hpp"

namespace mexcppclass {

/*! \brief Instance manager
 */
class InstanceManager : public InstanceManagerInterface {
 public:
  /*! \brief Defualt constructor
   */
  InstanceManager();

  /*! \brief Destructor
   */
  ~InstanceManager();

  /*! \brief Remove instance
   */
  void Erase(const InstanceId &id);

  /*! \brief Insert instance
   */
  InstanceId Insert(const InstancePointerInterface *pointer);

  /*! \brief Get instance
   */
  InstancePointerInterface *Pointer(const InstanceId &id) const;

  /*! \brief Clear
   */
  void Clear();

  /*! \brief Get stored instances
   */
  std::vector<InstanceId> GetInstances() const;

 private:
  /*! \brief Pair type
   */
  typedef std::pair<InstanceId, InstancePointer> InstancePointerPair;

  /*! \brief Handle instance and pointer map
   */
  typedef std::map<InstancePointerPair::first_type,
                   InstancePointerPair::second_type> InstancePointerMap;

  /*! \brief Matlab mex lock counter
  */
  unsigned int mex_lock_counter_ = 0;

  /*! \brief Instance map
   */
  InstancePointerMap instance_map_;
};

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_INSTANCEMANAGER_HPP_
