/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/modulemanager.hpp"

#include <Windows.h>

#include <functional>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "mexcppclass/moduleinterface.hpp"
#include "mexcppclass/moduleinterfacefunctionpointer.hpp"
#include "mexcppclass/matlab/memory.hpp"
#include "mexcppclass/matlab/string.hpp"
#include "mexcppclass/utility/string.hpp"
#include "mexcppclass/matlab/message.hpp"
#include "mexcppclass/matlab/messagedeque.hpp"
#include "mexcppclass/utility/map.hpp"

namespace mexcppclass {

// \TODO(rymut): delete function should be also used

void ModuleManager::RestoreLibrarySearchPath() {
  this->library_search_path_vector_.clear();
  #if defined(_WIN32)
  this->library_search_path_vector_.push_back(".\\");
  #else  // defined(_WIN32)
  this->library_search_path_vector_.push_back("./");
  #endif  // defined(_WIN32)
  matlab::MxArrayUniquePtr name_array(mxCreateString(mexFunctionName()));
  mxArray *input[1] = { name_array.get() };
  mxArray *output[1] = { nullptr };
  if (mexCallMATLAB(1, output, 1, input, "which") == 0) {
    if (output[0] != nullptr) {
      if (mxGetM(output[0]) == 1) {
        std::string library_file_path = matlab::StringFromMxArray(output[0]);

        std::size_t pos = library_file_path.rfind("\\");
        if (pos != std::string::npos) {
          // windows path
          this->library_search_path_vector_.push_back(
            library_file_path.substr(0, pos + 1));
        }
        pos = library_file_path.rfind("/");
        if (pos != std::string::npos) {
          // linux path
          this->library_search_path_vector_.push_back(
            library_file_path.substr(0, pos + 1));
        }
      } else {
        // \TODO(rymut): information about the error
      }
      mxDestroyArray(output[0]);
    }
  }
}

ModuleManager::ModuleManager() {
  this->library_id_object_count_map_[kModuleIdMexFunction] = 1;
  this->library_id_library_path_map_[kModuleIdMexFunction] =
    std::string(mexFunctionName());
  this->library_id_library_handle_map_[kModuleIdMexFunction] = nullptr;
  // relative position search path
  #if defined(_WIN32)
  this->library_search_path_vector_.push_back(".\\");
  #else  // defined(_WIN32)
  this->library_search_path_vector_.push_back("./");
  #endif  // defined(_WIN32)
  // absolute position search path
  matlab::MxArrayUniquePtr name_array(mxCreateString(mexFunctionName()));
  mxArray *input[1] = { name_array.get() };
  mxArray *output[1] = { nullptr };
  if (mexCallMATLAB(1, output, 1, input, "which") == 0) {
    if (output[0] != nullptr) {
      if (mxGetM(output[0]) == 1) {
        std::string library_file_path = matlab::StringFromMxArray(output[0]);
        this->library_id_object_count_map_[kModuleIdMexFunction] = 1;
        this->library_id_library_path_map_[kModuleIdMexFunction] =
          library_file_path;
        this->library_id_library_handle_map_[kModuleIdMexFunction] =
          GetModuleHandle(library_file_path.c_str());

        std::size_t pos = library_file_path.rfind("\\");
        if (pos != std::string::npos) {
          // windows path
          this->library_search_path_vector_.push_back(
            library_file_path.substr(0, pos + 1));
        }
        pos = library_file_path.rfind("/");
        if (pos != std::string::npos) {
          // linux path
          this->library_search_path_vector_.push_back(
            library_file_path.substr(0, pos + 1));
        }
      } else {
        // \TODO(rymut): information about the error
      }
      mxDestroyArray(output[0]);
    } else {
      // TODO(rymut): information about call fail
    }
  }
}
ModuleManager::~ModuleManager() {
  this->GarbageCollect();
    // \TODO(rymut): error it should be empty
}
// \TODO(rymut): check if i can use typedef here
template<typename T>
using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

// \TODO(rymut): move to separate header
class ModuleInterfaceDeleter {
 private:
  ModuleInterfaceDestroyFuncPtr destroy_func_ptr_;
 public:
  explicit ModuleInterfaceDeleter(ModuleInterfaceDestroyFuncPtr func) {
    this->destroy_func_ptr_ = func;
  }
  void operator()(ModuleInterface * p) const {
      destroy_func_ptr_(p);
  }
};

bool ModuleManager::IsFullnameValid(const std::string &fullname,
  std::string *synonym_ptr, std::string *type_ptr, std::string *library_ptr) {
  std::string synonym = "";
  std::string type = "";
  std::string library = "";
  if (synonym_ptr != nullptr) {
    *synonym_ptr = "";
  }
  if (type_ptr != nullptr) {
    *type_ptr = "";
  }
  if (library_ptr != nullptr) {
    *library_ptr = "";
  }
  auto pos_equal = fullname.find("=");
  if (pos_equal != std::string::npos) {
    synonym = fullname.substr(0, pos_equal);
    ++pos_equal;
  } else {
    pos_equal = 0;
  }
  auto pos_colon = fullname.find(":", pos_equal);
  if (pos_colon == std::string::npos) {
    type = fullname.substr(pos_equal);
  } else {
    type = fullname.substr(pos_equal, pos_colon - pos_equal);
    ++pos_colon;
    library = fullname.substr(pos_colon);
  }
  if (synonym.empty() == true) {
    synonym = type;
  }
  if (utility::IsAlphaNumeric(synonym) == false ||
    utility::IsAlphaNumeric(type) == false ||
    type.empty() == true || synonym.empty() == true ) {
    return false;
  }
  if (synonym_ptr != nullptr) {
    *synonym_ptr = synonym;
  }
  if (type_ptr != nullptr) {
    *type_ptr = type;
  }
  if (library_ptr != nullptr) {
    *library_ptr = library;
  }
  return true;
}

// \TODO(rymut): this should evaluate absolute path (remove . and ..)
ModuleId ModuleManager::GetModuleTypeFunctions(const std::string &name,
  const std::string &path,
  ModuleInterfaceCreateFuncPtr * create_func_ptr_result,
  ModuleInterfaceDestroyFuncPtr * destroy_func_ptr_result) {
  if (create_func_ptr_result != nullptr) {
    *create_func_ptr_result = nullptr;
  }
  if (destroy_func_ptr_result != nullptr) {
    *destroy_func_ptr_result = nullptr;
  }
  ModuleInterfaceCreateFuncPtr create_func_ptr = nullptr;
  ModuleInterfaceDestroyFuncPtr destroy_func_ptr = nullptr;

  std::string n = name;
  if (path.find("../") != std::string::npos ||
    path.find("..\\") != std::string::npos || n.empty() == true) {
    // \TODO(rymut): message
    return kModuleIdEmpty;
  }
  n[0] = ::toupper(n[0]);
  const std::string module_create  = "Create" + n + "ModuleInterface";
  const std::string module_destroy = "Destroy" + n + "ModuleInterface";

  // check loaded libraries - this also contains running mex function
  for (auto it : this->library_id_library_path_map_) {
    if (path.empty() || it.second == path) {
      HMODULE hmodule = this->library_id_library_handle_map_[it.first];
      FARPROC func_create = GetProcAddress(hmodule, module_create.c_str());
      FARPROC func_destroy = GetProcAddress(hmodule, module_destroy.c_str());
      create_func_ptr =
        reinterpret_cast<ModuleInterfaceCreateFuncPtr>(func_create);
      destroy_func_ptr =
        reinterpret_cast<ModuleInterfaceDestroyFuncPtr>(func_destroy);
      if (create_func_ptr != nullptr && destroy_func_ptr != nullptr) {
        if (create_func_ptr_result != nullptr) {
          *create_func_ptr_result = create_func_ptr;
        }
        if (destroy_func_ptr_result != nullptr) {
          *destroy_func_ptr_result = destroy_func_ptr;
        }
        return it.first;
      }
    }
  }

  // search in new libraries
  for (auto it : this->library_search_path_vector_) {
    const std::string hmodule_path = it + path;
    HMODULE hmodule = LoadLibrary(hmodule_path.c_str());
    if (hmodule != nullptr) {
      FARPROC func_create = GetProcAddress(hmodule, module_create.c_str());
      FARPROC func_destroy = GetProcAddress(hmodule, module_destroy.c_str());
      create_func_ptr =
        reinterpret_cast<ModuleInterfaceCreateFuncPtr>(func_create);
      destroy_func_ptr =
        reinterpret_cast<ModuleInterfaceDestroyFuncPtr>(func_destroy);
      if (create_func_ptr != nullptr && destroy_func_ptr != nullptr) {
        // register module
        ModuleId library_id = 1;
        for (auto it : this->library_id_library_handle_map_) {
          auto diff = it.first - library_id;
          if (diff > 1) {
            break;
          }
          library_id = it.first;
        }
        ++library_id;
        // register module
        this->library_id_library_handle_map_[library_id] = hmodule;
        this->library_id_library_path_map_[library_id] = hmodule_path;
        this->library_id_object_count_map_[library_id] = 0;
        if (create_func_ptr_result != nullptr) {
          *create_func_ptr_result = create_func_ptr;
        }
        if (destroy_func_ptr_result != nullptr) {
          *destroy_func_ptr_result = destroy_func_ptr;
        }
        return library_id;
      }
      FreeLibrary(hmodule);
    }
  }
  return kModuleIdEmpty;
}

const std::vector<std::string> &ModuleManager::GetLibrarySearchPath() const {
  return this->library_search_path_vector_;
}

void ModuleManager::InsertLibrarySearchPath(const std::string &path) {
  auto it = std::find(this->library_search_path_vector_.begin(),
    this->library_search_path_vector_.end(), path);
  if (it == this->library_search_path_vector_.end()) {
    this->library_search_path_vector_.push_back(path);
  }
}

void ModuleManager::EraseLibrarySearchPath(const std::string &path) {
  auto it = std::find(this->library_search_path_vector_.begin(),
    this->library_search_path_vector_.end(), path);
  if (it != this->library_search_path_vector_.end()) {
    this->library_search_path_vector_.erase(it);
  }
}

static std::string StringRemoveLongestPrefix(const std::string &text,
  const std::vector<std::string> &prefixes) {
  std::string result = text;
  for (const auto &prefix : prefixes) {
    auto pos = text.find(prefix);
    if (pos == 0) {
      std::string tmp = text.substr(prefix.length());
      if (tmp.length() < result.length()) {
        result = tmp;
      }
    }
  }
  return result;
}
InstancePointerInterface *ModuleManager::CreateInstance(
  const char *synonym_or_fullname) {
  matlab::MessageDeque deque;  // \TODO (rymut): temporary solution
  if (synonym_or_fullname == nullptr) {
    // TODO(rymut): error message
    return nullptr;
  }
  std::string synonym = "", type = "", library = "";
  if (IsFullnameValid(synonym_or_fullname, &synonym, &type, &library)
    == false) {
    // TODO(rymut): error message
    return nullptr;
  }
  auto pair =
    utility::MapValue(this->synonym_name_and_library_id_map_, synonym);
  if (pair.has_value()) {  // check if sysonym exists
    auto value = pair.value();
    // \TODO(rymut): different type of validation
    std::string library_without_prefix = StringRemoveLongestPrefix(
      this->library_id_library_path_map_[value.second],
      this->library_search_path_vector_);
    if ((type.empty() || value.first == type) && (library.empty() ||
      this->library_id_library_path_map_[value.second] == library ||
      library_without_prefix == library)) {
      type = value.first;
      library = this->library_id_library_path_map_[value.second];
    } else {  // library mismatch
      deque.push_back(matlab::Message(matlab::MsgId::Custom, "modulemanager",
        "type or/and library mismatch"));
      deque.Display(matlab::MsgLevel::Error);
      return nullptr;
    }
  }

  ModuleInterfaceCreateFuncPtr create_func_ptr = nullptr;
  ModuleInterfaceDestroyFuncPtr destroy_func_ptr = nullptr;
  ModuleId module_id = GetModuleTypeFunctions(type, library,
    &create_func_ptr, &destroy_func_ptr);
  if (module_id == kModuleIdEmpty) {
    // TODO(rymut): message
    this->GarbageCollect();  // unload loaded libraries
    return nullptr;
  }
  auto result_ptr = create_func_ptr();
  if (result_ptr != nullptr) {
    InstancePointer result(InstancePointer::SmartPointer(result_ptr,
      ModuleInterfaceDeleter(destroy_func_ptr)));
    ++this->library_id_object_count_map_[module_id];
    this->synonym_name_and_library_id_map_[synonym] =
      std::make_pair(type, module_id);
    this->object_library_id_map_[result] = module_id;
    // TODO(rymut): object needs to be deleted after usage write documentation
    // or method DeleteInstance(InstancePointerInterface *obj)
    return new InstancePointer(result);
  }
  destroy_func_ptr(result_ptr);
  result_ptr = nullptr;
  this->GarbageCollect();
  return nullptr;
}


InstancePointerInterface *ModuleManager::Manage(ModuleInterface *module,
  ModuleInterfaceDestroyFuncPtr destroy_func_ptr, ModuleInterface *parent) {
  if (module == nullptr || destroy_func_ptr == nullptr || parent == nullptr) {
    return nullptr;
  }
  for (auto &it : this->object_library_id_map_) {
    if (it.first.GetPointer() == parent) {
      InstancePointer result(InstancePointer::SmartPointer(module,
        destroy_func_ptr));
      ModuleId library_id = it.second;
      ++this->library_id_object_count_map_[library_id];
      this->object_library_id_map_[result] = library_id;
      // \TODO(rymut): delate after usage
      return new InstancePointer(result);
    }
  }
  return nullptr;
}

/*! \brief Unload unused modules
 */
void ModuleManager::GarbageCollect() {
  std::vector<InstancePointer> keys;
  for (auto &it : this->object_library_id_map_) {
    if (it.first.GetSmartPointer().use_count() <= 1) {
      keys.push_back(it.first);
      if (this->library_id_object_count_map_[it.second] > 0) {
        this->library_id_object_count_map_[it.second]--;
      }
    }
  }
  for (auto key : keys) {
    this->object_library_id_map_.erase(key);
  }
  keys.clear();

  std::vector<ModuleId> modules;
  for (auto &it : this->library_id_object_count_map_) {
    if (it.second == 0) {
      modules.push_back(it.first);
    }
  }

  for (auto &module : modules) {
    std::vector<std::string> synonyms;
    for (auto &it : this->synonym_name_and_library_id_map_) {
      if (it.second.second == module) {
        synonyms.push_back(it.first);
      }
    }
    for (auto synonym : synonyms) {
      this->synonym_name_and_library_id_map_.erase(synonym);
    }
    HMODULE module_handle = this->library_id_library_handle_map_[module];
    if (module != kModuleIdEmpty || module != kModuleIdMexFunction) {
      FreeLibrary(module_handle);
      this->library_id_object_count_map_.erase(module);
      this->library_id_library_handle_map_.erase(module);
      this->library_id_library_path_map_.erase(module);
    }
  }
}

}  // namespace mexcppclass
