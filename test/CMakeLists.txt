cmake_minimum_required(VERSION 3.16)

################################################################################
# Main script
################################################################################


macro(DownloadAndPopulateCatch2)
    set(CATCH_BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 self-test")
    set(BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 self-test")

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 0fa133a0c5e065065ef96ac2b6c0284cf5da265d
    )
    FetchContent_MakeAvailable(Catch2)
endmacro()


macro (CreateSmallRegisterRuntimeTests)
    add_executable(SmallRegisterTests
        ${CMAKE_CURRENT_LIST_DIR}/single_byte_setting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/single_byte_clearing.cpp
        ${CMAKE_CURRENT_LIST_DIR}/single_byte_clearing_whole_fields.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mutating_register.cpp
        ${CMAKE_CURRENT_LIST_DIR}/order_shall_not_matter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/obtain_individual_bitfields.cpp
        ${CMAKE_CURRENT_LIST_DIR}/abbreviated_forms.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mapping_old.cpp
    )
    target_link_libraries(SmallRegisterTests PRIVATE Catch2::Catch2WithMain SmallRegister)
    target_compile_features(SmallRegisterTests PRIVATE cxx_std_17)
    target_compile_options(SmallRegisterTests PRIVATE -Wall -Wextra)
    add_test(NAME SmallRegisterTestsRun COMMAND SmallRegisterTests)
endmacro()


macro (CreateSmallRegisterCompileTimeTests)
    include("${CMAKE_CURRENT_LIST_DIR}/static_assertions.cmake")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated1
        "small_register<bits<reg::one, 1>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated2
        "small_register<bits<reg::one, 4>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated3
        "small_register<bits<reg::one, 4>, bits<reg::two, 3>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated4
        "small_register<bits<reg::one, 3>, bits<reg::two, 3>, bits<reg::three, 1>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated5
        "small_register<bits<reg::one, 3>, bits<reg::two, 4>, bits<reg::three, 2>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated6
        "small_register<bits<reg::one, 9>>{}"
        ".*Accumulated bit-size must be 8.*")

    SmallRegister_AddStaticAssertionTest(no_duplicates_in_bitfield_names1
        "small_register<bits<reg::one, 3>, bits<reg::one, 5>>{}"
        ".*Bitfield names must be unique.*")

    SmallRegister_AddStaticAssertionTest(no_duplicates_in_bitfield_names2
        "small_register<bits<reg::one, 3>, bits<reg::two, 2>, bits<reg::one, 3>>{}"
        ".*Bitfield names must be unique.*")

    SmallRegister_AddStaticAssertionTest(cant_set_nonexisting_fields_for_single_set_arguments
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>> r; r.set(bitfield<reg::one>{0}, bitfield<reg::three>{1})"
        ".*Bitfield name not found.*")

    SmallRegister_AddStaticAssertionTest(cant_set_nonexisting_fields_for_multiple_set_arguments
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>> r; r.set(bitfield<reg::three>{1})"
        ".*Bitfield name not found.*")

    SmallRegister_AddStaticAssertionTest(cant_clear_nonexisting_fields
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>>{}.set().clear<reg::three>()"
        ".*Bitfield name not found.*")
    
    SmallRegister_AddStaticAssertionTest(cant_clear_nonexisting_fields_when_supplying_multiple_arguments
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>>{}.set().clear<reg::two, reg::three, reg::one>()"
        ".*Bitfield name not found.*")
    
    SmallRegister_AddStaticAssertionTest(cant_clear_individual_bits_in_nonexisting_fields
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>>{}.set().clear_individual_bits(bitfield<reg::three>{0})"
        ".*Bitfield name not found.*")

    SmallRegister_AddStaticAssertionTest(cant_clear_individual_bits_in_nonexisting_fields_for_multiple_args
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>>{}.set().clear_individual_bits(bitfield<reg::one>{0}, bitfield<reg::three>{0})"
        ".*Bitfield name not found.*")

    SmallRegister_AddStaticAssertionTest(cant_get_nonexisting_fields
        "small_register<bits<reg::one, 3>, bits<reg::two, 5>>{}.set().get<reg::three>()"
        ".*Bitfield name not found.*")

    SmallRegister_AddStaticAssertionTestWithOwnFile(cant_get_nonexisting_map_element
        ${CMAKE_CURRENT_LIST_DIR}/mapping_failed_compile_time.cpp
        ".*Register address not found.*")

endmacro()


macro (CreateSmallRegisterRefinedCompileTimeTests)

    add_executable(SmallRegisterRefinedTests
        ${CMAKE_CURRENT_LIST_DIR}/setting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/getting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/loading.cpp
        ${CMAKE_CURRENT_LIST_DIR}/clearing.cpp
        ${CMAKE_CURRENT_LIST_DIR}/chaining.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mapping.cpp
    )
    target_link_libraries(SmallRegisterRefinedTests PRIVATE Catch2::Catch2WithMain SmallRegisterRefined)
    target_compile_features(SmallRegisterRefinedTests PRIVATE cxx_std_17)
    target_compile_options(SmallRegisterRefinedTests PRIVATE -Wall -Wextra)
    add_test(NAME SmallRegisterRefinedTestsRun COMMAND SmallRegisterRefinedTests)
endmacro()

################################################################################
# Main script
################################################################################


DownloadAndPopulateCatch2()
CreateSmallRegisterRuntimeTests()
CreateSmallRegisterCompileTimeTests()
CreateSmallRegisterRefinedCompileTimeTests()

