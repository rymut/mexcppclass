/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_MODULEMANAGER_HPP_
#define MEXCPPCLASS_CORE_MODULEMANAGER_HPP_

#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <utility>
#include <vector>

// TODO(cpplint): shows this header as an error
#include "optional"
#include "mexcppclass/moduleid.hpp"
#include "mexcppclass/instanceid.hpp"
#include "mexcppclass/instancepointerinterface.hpp"
#include "mexcppclass/modulemanagerinterface.hpp"
#include "mexcppclass/moduleinterfacefunctionpointer.hpp"
#include "mexcppclass/matlab/messagedeque.hpp"
#include "instancepointer.hpp"

namespace mexcppclass {

enum class ModuleLocation {
  External, /*!< external dynamic module */
  Internal, /*!< internal registered module */
  Hidden    /*!< hidden module */
};

class ModuleManager : public ModuleManagerInterface {
 public:
  ModuleManager();

  ~ModuleManager();

  InstancePointerInterface *CreateInstance(const char *synonom_or_fullname);

  InstancePointerInterface *Manage(ModuleInterface *module,
    ModuleInterfaceDestroyFuncPtr destroy_func_ptr, ModuleInterface *parent);

  /*! \brief Unload unused modules
   */
  void GarbageCollect();

  // \TODO(rymut): i can probably use const before *
  ModuleId GetModuleTypeFunctions(const std::string &name,
    const std::string &path,
    ModuleInterfaceCreateFuncPtr * create_func_ptr,
    ModuleInterfaceDestroyFuncPtr * destroy_func_ptr);

  const std::vector<std::string> &GetLibrarySearchPath() const;

  void InsertLibrarySearchPath(const std::string &path);

  void EraseLibrarySearchPath(const std::string &path);

  void RestoreLibrarySearchPath();

 private:
  // fullname = [synonym=]class[:library]
  // \TODO(rymut): where to put const
  bool IsFullnameValid(const std::string &fullname,
    std::string *synonym, std::string *name, std::string *library);

  std::vector<std::string> library_search_path_vector_;
  std::map<std::string, std::pair<std::string, ModuleId>>
    synonym_name_and_library_id_map_;
  std::map<ModuleId, size_t> library_id_object_count_map_;
  std::map<ModuleId, std::string> library_id_library_path_map_;
  std::map<ModuleId, HMODULE> library_id_library_handle_map_;
  std::map<InstancePointer, ModuleId> object_library_id_map_;
};

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_MODULEMANAGER_HPP_
