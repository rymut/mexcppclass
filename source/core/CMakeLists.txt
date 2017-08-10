file(GLOB_RECURSE mexcppclasscore_SOURCES "*.cpp")
file(GLOB_RECURSE mexcppclasscore_HEADERS "*.hpp")
file(GLOB_RECURSE mexcppclasscore_INCLUDE
  "${PROJECT_SOURCE_DIR}/include/core/*.hpp")

add_library(mexcppclasscore OBJECT
  ${mexcppclasscore_SOURCES} ${mexcppclasscore_INCLUDE})
#add_dependencies(mexcppclasscore mexcppclassexport)
# include directories
target_include_directories(mexcppclasscore
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(mexcppclasscore
  PUBLIC "${PROJECT_SOURCE_DIR}/source")
target_include_directories(mexcppclasscore
  PUBLIC "${PROJECT_BINARY_DIR}/source")
target_include_directories(mexcppclasscore
  PUBLIC "${PROJECT_BINARY_DIR}/include")
target_include_directories(mexcppclasscore PUBLIC ${Matlab_INCLUDE_DIRS})
# compile definitions
target_compile_definitions(mexcppclasscore PUBLIC MATLAB_MEX_FILE=1)
target_compile_definitions(mexcppclasscore PUBLIC MEXCPPCLASS_EXPORTS=1)

set_property(TARGET ${mexcppclasscore} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(mexcppclasscore PROPERTIES
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
        target_compile_options(mexcppclasscore PUBLIC "/std:c++latest")
    endif()
endif()
