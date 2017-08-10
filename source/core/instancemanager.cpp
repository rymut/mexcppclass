/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/instancemanager.hpp"
// C standard headers
#include <mex.h>
// C++ standard headers
#include <string>
#include <vector>
// project headers
#include "mexcppclass/matlab/message.hpp"

namespace mexcppclass {

InstanceManager::InstanceManager() {
}

InstanceManager::~InstanceManager() {
  this->Clear();
}

void InstanceManager::Erase(const InstanceId &id) {
  InstancePointerMap::const_iterator it = this->instance_map_.find(id);

  if (it != this->instance_map_.end()) {
    this->instance_map_.erase(it);
    mexUnlock();
    --this->mex_lock_counter_;
  }
}

InstanceId InstanceManager::Insert(const InstancePointerInterface *interface) {
  const InstancePointer *pointer =
    dynamic_cast<const InstancePointer*>(interface);
  if (pointer == nullptr || pointer->GetPointer() == nullptr) {
    return kInstanceIdEmpty;
  }
  InstanceId id = kInstanceIdEmpty;
  // find nearest id
  for (auto it : this->instance_map_) {
    auto diff = it.first - id;
    if (diff > 1) {
      break;
    }
    id = it.first;
  }
  ++id;

  auto result = this->instance_map_.insert(
    InstancePointerPair(id, InstancePointer(*pointer)));

  if (!result.second) {
    matlab::Message::Display("InstanceManager:insert",
      "cannot register pointer", matlab::MsgId::Custom,
      matlab::MsgLevel::Warning);
    id = kInstanceIdEmpty;
  } else {
    mexLock();
    ++this->mex_lock_counter_;
  }
  return id;
}

InstancePointerInterface *InstanceManager::Pointer(const InstanceId &id) const {
  auto it = this->instance_map_.find(id);
  if (it == this->instance_map_.end()) {
    return nullptr;
  }
  // \TODO instance pointer needs to be deleted afterwards
  return new InstancePointer(it->second);
}

void InstanceManager::Clear() {
  while (this->mex_lock_counter_ >= 1) {
    mexUnlock();
    --this->mex_lock_counter_;
  }
  this->instance_map_.clear();
}

std::vector<InstanceId> InstanceManager::GetInstances() const {
  std::vector<InstanceId> result;
  for (const auto &it : this->instance_map_) {
    result.push_back(it.first);
  }
  return result;
}

}  // namespace mexcppclass
