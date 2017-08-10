
## UTILITY LIBRARY
file(GLOB_RECURSE mexcppclassutility_SOURCES "*.cpp")
file(GLOB_RECURSE mexcppclassutility_HEADERS "*.hpp")
file(GLOB_RECURSE mexcppclassutility_INCLUDE
  "${PROJECT_SOURCE_DIR}/include/utility/*.hpp")

add_library(mexcppclassutility ${mexcppclassutility_SOURCES}
  ${mexcppclassutility_HEADERS} ${mexcppclassutility_INCLUDE})

target_include_directories(mexcppclassutility
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(mexcppclassutility
  PUBLIC "${PROJECT_BINARY_DIR}/source")
target_include_directories(mexcppclassutility
  PUBLIC "${PROJECT_BINARY_DIR}/include")
## CZY TO JEST POTRZEBNA

export(TARGETS mexcppclassutility
  FILE "${CMAKE_BINARY_DIR}/MexcppclassUtilityTargets.cmake")
export(PACKAGE mexcppclassutility)

install(TARGETS mexcppclassutility DESTINATION lib)
install(FILES ${mexcppclassutility_INCLUDE} DESTINATION include)

set(target_NAME "mexcppclassutility")
set(mexcppclassutility_INCLUDE_DIRS "${mexcppclassutility_INCLUDE}")
set(CMAKE_CONFIG_DEST "share/cmake/${PROJECT_NAME}")
include(CMakePackageConfigHelpers)
configure_package_config_file (
  "${PROJECT_SOURCE_DIR}/cmake/MexcppclassConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/MexcppclassUtilityConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_CONFIG_DEST}"
  PATH_VARS mexcppclassutility_INCLUDE_DIRS )
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/MexcppclassUtilityConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES
  "${PROJECT_BINARY_DIR}/MexcppclassUtilityConfig.cmake"
  "${PROJECT_BINARY_DIR}/MexcppclassUtilityConfigVersion.cmake"
  DESTINATION "lib/${PROJECT_NAME}/cmake")

# Other settings
set_target_properties(mexcppclassutility PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
string(TOLOWER ${CMAKE_BUILD_TOOL} BUILD_TOOL_LOWERCASE)
if(BUILD_TOOL_LOWERCASE MATCHES "(msdev|devenv|nmake|msbuild)")
#  target_compile_options(mexcppclassutility PUBLIC  "/W4" )
endif()
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(mexcppclassutility PUBLIC "/std:c++latest")
    endif()
endif()
