/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_PROGRAM_HPP_
#define MEXCPPCLASS_CORE_PROGRAM_HPP_

#include <mex.h>
#include <Windows.h>
#include <map>
#include <string>
#include "instancemanager.hpp"
#include "modulemanager.hpp"

namespace mexcppclass {

/*! \brief Mex program wrapper
 */
class Program {
 public:
  /*! \brief Default constructor
  */
  Program();

  /*! \brief Destructor
  */
  ~Program();

  /*! \brief Command 'commands' implementation
   *  \summary return available commands
   *    Usage:
   *      [commands] = mexfunc('commands')
   */
  void CommandCommands(NonpersistentDataInterface *args);

  /*! \brief Command 'create' implementation
   *  \summary create object of specified synonym type or direct type
   *    Usage:
   *      [handle] = mexfunc('create', 'synonym_or_internal_type')
   *      [handle] = mexfunc('create', '[synonym=]type[:shared_module_path]')
   */
  void CommandCreate(NonpersistentDataInterface *args);

  /*! \brief Command 'destroy' implementation
   *  \summary destroy object handle
   *    [] = mexfunction('destroy', handle)
   */
  void CommandDestroy(NonpersistentDataInterface *args);

  /*! \brief Command 'version' implementation
   *  \summary get program version number
   *    Usage:
   *      [name [major [minor [patch [tweak]]]]] = mexfunc('version')
   */
  void CommandVersion(NonpersistentDataInterface *args);

  /*! \brief Command 'handle' implementation
   *  \summary check if handle is an object
   *    Usage:
   *      [logical] = mexfunction('handle', handle)
   *
   *      logical - true if handle is valid false otherwise
   */
  void CommandHandle(NonpersistentDataInterface *args);

  /*! \brief Command 'clear' implementation
   *  \summary clear all resources and unlock mex file
   *    Usage:
   *      [] = mexfunc('clear');
   */
  void CommandClear(NonpersistentDataInterface *args);

  /*! \brief Command 'methods' implementation
   *  \summary get object/class methods
   *    Usage:
   *      [methods] = mexfunc('methods', handle)
   *      [methods] = mexfunc('methods', 'type')
   */
  void CommandMethods(NonpersistentDataInterface *args);

  /*! \brief Command 'types' return registered types
   *  \summary Get registered types:
   *    Usage:
   *      [types] = mexfunc('types');
   */
  void CommandTypes(NonpersistentDataInterface *args);

  /*! \brief Get type of the handle
   *  \summary
   *    Usage:
   *      [type] = mexfunc('type', handle)
   */
  void CommandType(NonpersistentDataInterface *args);

  /*! \brief Command 'objects' implementation
   *  \summary Get all registered handles
   *    Usage:
   *      [handles] = mexfunc('objects');
   */
  void CommandObjects(NonpersistentDataInterface *args);

  /*! \brief Command 'call' implementation
   *  \summary call method on the handle
   *    Usage:
   *      [argout] = mexfunc('call', handle, method, [argin]);
   */
  void CommandCall(NonpersistentDataInterface *args);

  /*! \brief Command 'libpath' implementation
   *  \summary manipulate library search paths
   *    Usage:
   *      [paths] = mexfunc('libpath');
   *      [paths] = mexfunc('libpath', 'add', path);
   *      [paths] = mexfunc('libpath', 'del', path);
   *      [paths] = mexfunc('libpath', 'reset');
   */
  void CommandLibpath(NonpersistentDataInterface *args);

  void Main(const int &input_count, const mxArray *const *const input,
    const int &output_count, mxArray **output);

 private:
    InstanceManager instance_mgr_;
    ModuleManager module_mgr_;
};

}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_PROGRAM_HPP_
