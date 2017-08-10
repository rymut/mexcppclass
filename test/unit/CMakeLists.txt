find_package(Matlab "9.1" OPTIONAL_COMPONENTS MAIN_PROGRAM REQUIRED)

file(GLOB_RECURSE test_module_SRC "*.cpp")
file(GLOB_RECURSE test_module_INC "*.hpp")
add_library(test_module MODULE ${test_module_INC} ${test_module_SRC})


set_target_properties(test_module PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(test_module PUBLIC "/std:c++latest")
    endif()
endif()

target_link_libraries(test_module ${Matlab_MEX_LIBRARY})
target_link_libraries(test_module ${Matlab_MX_LIBRARY})
target_link_libraries(test_module mexcppclassutility)
target_link_libraries(test_module mexcppclassmatlab)
target_link_libraries(test_module mexcppclass)
target_include_directories(test_module PUBLIC "${PROJECT_BINARY_DIR}/source")

target_include_directories(test_module PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(test_module PUBLIC "${PROJECT_BINARY_DIR}/source")
target_include_directories(test_module PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(test_module PUBLIC ${Matlab_INCLUDE_DIRS})

include("copy_to_target_path")
copy_to_target_path(test_module
  "$<TARGET_FILE_DIR:mexcppclass>" "$<TARGET_FILE_NAME:mexcppclass>")
copy_to_target_path(test_module
  "${CMAKE_CURRENT_BINARY_DIR}" "setup_unit_test_properties.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "BaseUnitTest.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "run_unit_test.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "CommandCommandsUnitTest.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "CommandLibpathUnitTest.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "CommandVersionUnitTest.m")
#copy_to_target_path(test_module
#  "${CMAKE_CURRENT_SOURCE_DIR}" "GeneralUnitTest.m")

install(TARGETS test_module DESTINATION bin)

set(MEX_VERSION "${PROJECT_VERSION}")
set(MEX_NAME "${PROJECT_NAME}")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/setup_unit_test_properties.m.in"
  "${CMAKE_CURRENT_BINARY_DIR}/setup_unit_test_properties.m")

matlab_add_unit_test(NAME GeneralUnitTest
  UNITTEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/GeneralUnitTest.m"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  CUSTOM_TEST_COMMAND
  "exit(run_unit_test('GeneralUnitTest', 'setup_unit_test_properties'))"
  ADDITIONAL_PATH "$<TARGET_FILE_DIR:test_module>" "${CMAKE_CURRENT_SOURCE_DIR}"
  TEST_ARGS "CONFIGURATIONS Release")
set_tests_properties(GeneralUnitTest PROPERTIES DEPENDS test_module)

matlab_add_unit_test(NAME CommandCommandsUnitTest
  UNITTEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CommandCommandsUnitTest.m"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  CUSTOM_TEST_COMMAND
  "exit(run_unit_test('CommandCommandsUnitTest', 'setup_unit_test_properties'))"
  ADDITIONAL_PATH "$<TARGET_FILE_DIR:test_module>" "${CMAKE_CURRENT_SOURCE_DIR}"
  TEST_ARGS "CONFIGURATIONS Release")
set_tests_properties(GeneralUnitTest PROPERTIES DEPENDS test_module)


matlab_add_unit_test(NAME CommandLibpathUnitTest
  UNITTEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CommandLibpathUnitTest.m"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  CUSTOM_TEST_COMMAND
  "exit(run_unit_test('CommandLibpathUnitTest', 'setup_unit_test_properties'))"
  ADDITIONAL_PATH "$<TARGET_FILE_DIR:test_module>" "${CMAKE_CURRENT_SOURCE_DIR}"
  TEST_ARGS "CONFIGURATIONS Release")
set_tests_properties(GeneralUnitTest PROPERTIES DEPENDS test_module)

matlab_add_unit_test(NAME CommandVersionUnitTest
  UNITTEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CommandVersionUnitTest.m"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  CUSTOM_TEST_COMMAND
  "exit(run_unit_test('CommandVersionUnitTest', 'setup_unit_test_properties'))"
  ADDITIONAL_PATH "$<TARGET_FILE_DIR:test_module>" "${CMAKE_CURRENT_SOURCE_DIR}"
  TEST_ARGS "CONFIGURATIONS Release")
set_tests_properties(GeneralUnitTest PROPERTIES DEPENDS test_module)
