cmake_minimum_required(VERSION 3.20)
project(ASC_ode)

set (CMAKE_CXX_STANDARD 20)

include(CTest)

include_directories(src nanoblas/src)

add_subdirectory (nanoblas)


add_executable (test_ode demos/test_ode.cpp)
target_link_libraries (test_ode PUBLIC nanoblas)

add_executable (demo_autodiff demos/demo_autodiff.cpp)
target_link_libraries (demo_autodiff PUBLIC nanoblas)

add_executable (test_order demos/test_order.cpp)
target_link_libraries (test_order PUBLIC nanoblas)

add_executable (test_stiff demos/test_stiff.cpp)
target_link_libraries (test_stiff PUBLIC nanoblas)

if (BUILD_TESTING)
  add_test (NAME test_order COMMAND test_order)
  add_test (NAME test_stiff COMMAND test_stiff)
  set_tests_properties (test_order test_stiff
    PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

