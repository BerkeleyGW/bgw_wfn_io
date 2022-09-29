cmake_minimum_required (VERSION 3.10)

project (
  BerkeleyGW_WFN_IO
  VERSION 0.1
  LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(HDF5 REQUIRED COMPONENTS C HL)

add_library (bgw_wfn_io bgw_wfn_io.c bgw_wfn_io.h)

target_include_directories (bgw_wfn_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (test_bindings.x test_bindings.c bgw_wfn_io.c bgw_wfn_io.h)

#target_link_libraries (test_bindings.x LINK_PUBLIC bgw_wfn_io)
target_link_libraries (test_bindings.x PRIVATE hdf5::hdf5_hl hdf5::hdf5)
