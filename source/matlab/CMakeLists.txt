file(GLOB_RECURSE mexcppclassmatlab_SOURCES "*.cpp")
file(GLOB_RECURSE mexcppclassmatlab_HEADERS "*.hpp")
file(GLOB_RECURSE mexcppclassmatlab_INCLUDE
  "${PROJECT_SOURCE_DIR}/include/matlab/*.hpp")

add_library(mexcppclassmatlab ${mexcppclassmatlab_SOURCES}
  ${mexcppclassmatlab_HEADERS} ${mexcppclassmatlab_INCLUDE})

target_include_directories(mexcppclassmatlab
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(mexcppclassmatlab
  PUBLIC "${PROJECT_BINARY_DIR}/source")
target_include_directories(mexcppclassmatlab
  PUBLIC "${PROJECT_BINARY_DIR}/include")
target_include_directories(mexcppclassmatlab
  PUBLIC ${Matlab_INCLUDE_DIRS})

install(TARGETS mexcppclassmatlab DESTINATION lib)
install(FILES ${mexcppclassmatlab_INCLUDE} DESTINATION include)

export(TARGETS mexcppclassmatlab
  FILE "${CMAKE_BINARY_DIR}/MexcppclassMatlabTargets.cmake" )
export(PACKAGE mexcppclassmatlab)

set(target_NAME "mexcppclassmatlab")
set(mexcppclassmatalb_INCLUDE_DIRS "${mexcppclassmatlab_INCLUDE}")
set(CMAKE_CONFIG_DEST "share/cmake/${PROJECT_NAME}")
include(CMakePackageConfigHelpers)
configure_package_config_file (
  "${PROJECT_SOURCE_DIR}/cmake/MexcppclassConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/MexcppclassMatlabConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_CONFIG_DEST}"
  PATH_VARS mexcppclassmatalb_INCLUDE_DIRS )
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/MexcppclassMatlabConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES
  "${PROJECT_BINARY_DIR}/MexcppclassMatlabConfig.cmake"
  "${PROJECT_BINARY_DIR}/MexcppclassMatlabConfigVersion.cmake"
  DESTINATION "lib/${PROJECT_NAME}/cmake")

set_target_properties(mexcppclassmatlab PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
string(TOLOWER ${CMAKE_BUILD_TOOL} BUILD_TOOL_LOWERCASE)
if(BUILD_TOOL_LOWERCASE MATCHES "(msdev|devenv|nmake|msbuild)")
#  target_compile_options(mexcppclassmatlab PUBLIC  "/W4" )
endif()
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(mexcppclassmatlab PUBLIC "/std:c++latest")
    endif()
endif()
