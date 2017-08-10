file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/export.cpp" "")
add_library(mexcppclassexport SHARED "${CMAKE_CURRENT_BINARY_DIR}/export.cpp")

set_target_properties(mexcppclassexport PROPERTIES
  DEFINE_SYMBOL "MEXCPPCLASS_EXPORTS")

include(GenerateExportHeader)
generate_export_header(mexcppclassexport
  EXPORT_MACRO_NAME MEXCPPCLASS_EXPORT
EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/mexcppclass/export.hpp)
export(TARGETS mexcppclassexport
  FILE "${CMAKE_BINARY_DIR}/MexcppclassExport.cmake")
install(FILES "${PROJECT_BINARY_DIR}/include/mexcppclass/export.hpp"
  DESTINATION "include")
#install(FILES "${CMAKE_BINARY_DIR}/MexcppclassExport.cmake"
#  DESTINATION "lib/${PROJECT_NAME}/cmake")

add_subdirectory(utility)
add_subdirectory(matlab)
add_subdirectory(core)


matlab_add_mex(NAME mexcppclass SHARED "mexcppclass.cpp"
  $<TARGET_OBJECTS:mexcppclasscore>)
if (WIN32)
#    set_target_properties(mexcppclass PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:" )
elseif (APPLE)
    set_target_properties(mexcppclass PROPERTIES
        LINK_FLAGS "-Wl,-all_load"
    )
else ()
    set_target_properties(mexcppclass PROPERTIES
        LINK_FLAGS "-Wl,--whole-archive"
    )
endif ()
#add_dependencies(mexcppclass mexcppclassexport)
#add_definitions(())
#  $<TARGET_OBJECTS:mexcppclasscore>)

set_target_properties(mexcppclass PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

#if (MSVC_VERSION)
string(TOLOWER ${CMAKE_BUILD_TOOL} BUILD_TOOL_LOWERCASE)
if(BUILD_TOOL_LOWERCASE MATCHES "(msdev|devenv|nmake|msbuild)")
#  target_compile_options(mexcppclass PUBLIC  "/W4" )
endif()
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(mexcppclass PUBLIC "/std:c++latest")
    endif()
endif()
target_include_directories(mexcppclass PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(mexcppclass PUBLIC "${PROJECT_BINARY_DIR}/source")
target_include_directories(mexcppclass PUBLIC "${PROJECT_BINARY_DIR}/include")
target_include_directories(mexcppclass PUBLIC ${Matlab_INCLUDE_DIRS})
target_compile_definitions(mexcppclass PUBLIC MATLAB_MEX_FILE=1)
target_compile_definitions(mexcppclass PUBLIC MEXCPPCLASS_EXPORTS=1)

target_link_libraries(mexcppclass mexcppclassutility)
target_link_libraries(mexcppclass mexcppclassmatlab)
target_link_libraries(mexcppclass ${Matlab_MEX_LIBRARY})
target_link_libraries(mexcppclass ${Matlab_MX_LIBRARY})

install(TARGETS mexcppclass DESTINATION bin)
install(FILES ${mexcppclass_HEADERS} DESTINATION include)

set(mexcppclass_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" )
set(CMAKE_CONFIG_DEST "share/cmake/${PROJECT_NAME}")

include(CMakePackageConfigHelpers)
configure_package_config_file (
  "${PROJECT_SOURCE_DIR}/cmake/MexcppclassConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/MexcppclassConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_CONFIG_DEST}"
  PATH_VARS mexcppclass_INCLUDE_DIRS )
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/MexcppclassConfigVersion.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion)

install(FILES
  "${PROJECT_BINARY_DIR}/MexcppclassConfig.cmake"
  "${PROJECT_BINARY_DIR}/MexcppclassConfigVersion.cmake"
  DESTINATION "lib/${PROJECT_NAME}/cmake")
# This file is included in our template:
export(TARGETS mexcppclass FILE "${CMAKE_BINARY_DIR}/MexcppclassTargets.cmake")
export(PACKAGE mexcppclass)
