/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_CORE_INSTANCEPOINTER_HPP_
#define MEXCPPCLASS_CORE_INSTANCEPOINTER_HPP_

#include <mex.h>
#include <memory>
#include "mexcppclass/instancepointerinterface.hpp"
#include "mexcppclass/moduleinterface.hpp"

namespace mexcppclass {

class InstancePointer : public InstancePointerInterface {
 public:
  /*! \brief Handle pointer
   */
  typedef std::shared_ptr<ModuleInterface> SmartPointer;

  /* !\brief Default constructor
   */
  InstancePointer();

  /*! \brief Copy constructor
   */
  InstancePointer(const InstancePointer &rhs);

  /*! \brief Create instance pointer from smart pointer
   */
  explicit InstancePointer(const SmartPointer &pointer);

  /*! \brief Create instance pointer from normal pointer
   */
  explicit InstancePointer(Pointer pointer);

  /*! \brief Destructor
   */
  ~InstancePointer();

  /*! \brief Greather compare operator
   *  \summary compare stored raw pointer address
   */
  bool operator<(const InstancePointer &rhs) const;

  /*! \brief Assign operator
   */
  InstancePointer &operator=(const InstancePointer &rhs);

  /*! \brief Get raw pointer
   */
  virtual Pointer GetPointer() const;

  /*! \brief Get smart pointer
   */
  const SmartPointer &GetSmartPointer() const;

 private:
  /*! \brief Smart pointer holding object
   */
  SmartPointer smart_pointer_;
};

// \TODO (rymut): remove from this header
inline void DeleteInstancePointer(InstancePointerInterface *obj) {
  InstancePointer *pointer = dynamic_cast<InstancePointer*>(obj);
  if (pointer != nullptr) {
    delete pointer;
  } else {
    // \TODO(rymut): message ("dynamic_cast<InstancePointer*> failed\n");
  }
}
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_CORE_INSTANCEPOINTER_HPP_
