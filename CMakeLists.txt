cmake_minimum_required(VERSION 3.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)
include(version)
version_configure("${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VER VER_BUILD)


project(mexcppclass VERSION ${VER})
enable_testing()

version_make_file(${PROJECT_BINARY_DIR}/source/utility/version.h)

option(ENABLE_MEXCPPCLASS_CMAKE_ENV_SCRIPT_FILE
  "Enable cmake enviroment variable file to config enviroment"
  ON
)

set(MEXCPPCLASS_CMAKE_ENV_SCRIPT_FILE
  "${CMAKE_SOURCE_DIR}/cmake/enviroment/script.cmake"
  CACHE STRING "aaa CMAKE enviroment variable file to config"
)

# Load enviroment variables
if(ENABLE_MEXCPPCLASS_CMAKE_ENV_SCRIPT_FILE)
  include(${MEXCPPCLASS_CMAKE_ENV_SCRIPT_FILE})
endif()

# Find OpenGL libraries
find_package(Matlab "9.1" OPTIONAL_COMPONENTS MX_LIBRARY REQUIRED)
add_subdirectory(source)
add_subdirectory(test "test")
