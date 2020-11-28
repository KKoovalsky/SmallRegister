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


macro (CreateSmallRegisterCompileTimeTests)
    include("${CMAKE_CURRENT_LIST_DIR}/static_assertions.cmake")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated1
        "small_register<uint8_t, bitfield<reg::one, 1>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated2
        "small_register<uint8_t, bitfield<reg::one, 4>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated3
        "small_register<uint8_t, bitfield<reg::one, 4>, bitfield<reg::two, 3>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated4
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 3>, bitfield<reg::three, 1>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated5
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 4>, bitfield<reg::three, 2>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(all_bits_must_be_allocated6
        "small_register<uint8_t, bitfield<reg::one, 9>>{}"
        ".*Whole register must be allocated.*")

    SmallRegister_AddStaticAssertionTest(no_duplicates_in_bitfield_names1
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::one, 5>>{}"
        ".*Bitfield IDs must be unique.*")

    SmallRegister_AddStaticAssertionTest(no_duplicates_in_bitfield_names2
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 2>, bitfield<reg::one, 3>>{}"
        ".*Bitfield IDs must be unique.*")

    SmallRegister_AddStaticAssertionTest(cant_set_nonexisting_bitfield
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 5>> r; r.set<reg::three>(); "
        ".*Bitfield ID not found.*")

    SmallRegister_AddStaticAssertionTest(cant_clear_nonexisting_fields
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 5>> r; r.clear<reg::three>(); "
        ".*Bitfield ID not found.*")

    SmallRegister_AddStaticAssertionTest(cant_get_nonexisting_fields
        "small_register<uint8_t, bitfield<reg::one, 3>, bitfield<reg::two, 5>> r; r.get<reg::three>(); "
        ".*Bitfield ID not found.*")

    SmallRegister_AddStaticAssertionTestWithOwnFile(cant_get_nonexisting_map_element
        ${CMAKE_CURRENT_LIST_DIR}/mapping_failed_compile_time.cpp
        ".*Register address not found.*")

    SmallRegister_AddStaticAssertionTestWithOwnFile(cant_have_register_ids_with_various_types
        ${CMAKE_CURRENT_LIST_DIR}/wrong_types_of_register_ids_compile_time.cpp
        ".*bitfield::id types shall be the same.*")

endmacro()


macro (CreateSmallRegisterRuntimeTimeTests)
    add_executable(SmallRegisterTests
        ${CMAKE_CURRENT_LIST_DIR}/setting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/getting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/loading.cpp
        ${CMAKE_CURRENT_LIST_DIR}/clearing.cpp
        ${CMAKE_CURRENT_LIST_DIR}/chaining.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mapping.cpp
    )
    target_link_libraries(SmallRegisterTests PRIVATE Catch2::Catch2WithMain SmallRegister)
    target_compile_features(SmallRegisterTests PRIVATE cxx_std_17)
    target_compile_options(SmallRegisterTests PRIVATE -Wall -Wextra)
    add_test(NAME SmallRegisterTestsRun COMMAND SmallRegisterTests)
endmacro()

################################################################################
# Main script
################################################################################


DownloadAndPopulateCatch2()
CreateSmallRegisterCompileTimeTests()
CreateSmallRegisterRuntimeTimeTests()

