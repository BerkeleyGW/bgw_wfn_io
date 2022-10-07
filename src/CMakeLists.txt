
set(HEADER_LIST "${BerkeleyGW_WFN_IO_SOURCE_DIR}/include/bgw_wfn_io.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(bgw_wfn_io bgw_wfn_io.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(bgw_wfn_io PUBLIC ../include)

# This depends on HDF5
target_link_libraries(bgw_wfn_io PUBLIC hdf5::hdf5_hl hdf5::hdf5)

install(TARGETS bgw_wfn_io DESTINATION lib)
install(FILES ${HEADER_LIST} DESTINATION include)
