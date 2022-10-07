
# Tests need to be added as executables first
add_executable(test_bindings.x test_bindings.c)

# Should be linked to the main library
target_link_libraries(test_bindings.x PRIVATE bgw_wfn_io)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME test_bindings_c COMMAND test_bindings.x) # Command can be a target
add_test(NAME test_bindings_py COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_bindings.py $<TARGET_FILE:bgw_wfn_io>)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/WFN_in.h5
    ${CMAKE_CURRENT_BINARY_DIR}/
    USE_SOURCE_PERMISSIONS COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bgw_wfn.py
    ${CMAKE_CURRENT_BINARY_DIR}/
    USE_SOURCE_PERMISSIONS COPYONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_bindings.py
    ${CMAKE_CURRENT_BINARY_DIR}/
    USE_SOURCE_PERMISSIONS COPYONLY
)
