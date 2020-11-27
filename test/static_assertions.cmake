function(SmallRegister_AddStaticAssertionTest name body pass_regex)

    # Used within the *.in file
    set(SMALL_REGISTER_STATIC_ASSERT_TEST_BODY ${body})
    set(output_file_name static_assertion_test_${name}.cpp)
    configure_file(static_assertion_test.cpp.in ${output_file_name})

    set(test_name SmallRegister_StaticAssertionTest_${name})
    set(test_run_name ${test_name}_run)
    add_executable(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${output_file_name})

    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(${test_name} PRIVATE SmallRegister)
    set_target_properties(${test_name} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${test_run_name} COMMAND 
        ${CMAKE_COMMAND} --build . --target ${test_name} --config $<CONFIGURATION>)
    set_tests_properties(${test_run_name} PROPERTIES PASS_REGULAR_EXPRESSION ${pass_regex})

endfunction()

function(SmallRegister_AddStaticAssertionTestWithOwnFile name filename pass_regex)

    set(test_name SmallRegister_StaticAssertionTest_${name})
    set(test_run_name ${test_name}_run)
    add_executable(${test_name} ${filename})

    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(${test_name} PRIVATE SmallRegister)
    set_target_properties(${test_name} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${test_run_name} COMMAND 
        ${CMAKE_COMMAND} --build . --target ${test_name} --config $<CONFIGURATION>)
    set_tests_properties(${test_run_name} PROPERTIES PASS_REGULAR_EXPRESSION ${pass_regex})

endfunction()
