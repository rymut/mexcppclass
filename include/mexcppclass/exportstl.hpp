/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_EXPORTSTL_HPP_
#define MEXCPPCLASS_EXPORTSTL_HPP_

#include "mexcppclass/export.hpp"

#ifndef MEXCPPCLASS_STATIC_DEFINE
#    define MEXCPPCLASS_EXPORT_TEMPLATE
#else
#    define MEXCPPCLASS_EXPORT_TEMPLATE extern
#endif

/* example of exporting template std::vector
#define MEXCPPCLASS_EXPORT_TEMPLATE_STL_VECTOR( vectype ) \
  MEXCPPCLASS_EXPORT_TEMPLATE template class MEXCPPCLASS_EXPORT \
    std::allocator< vectype >; \
  MEXCPPCLASS_EXPORT_TEMPLATE template class MEXCPPCLASS_EXPORT \
    std::vector<vectype, std::allocator< vectype > >;
*/

#endif  // MEXCPPCLASS_EXPORTSTL_HPP_
