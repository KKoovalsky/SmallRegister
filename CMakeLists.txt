cmake_minimum_required(VERSION 3.16)

################################################################################
# Macros
################################################################################


macro(CreateMainRefinedTarget)
    add_library(SmallRegisterRefined INTERFACE)
    target_include_directories(SmallRegisterRefined INTERFACE ${CMAKE_CURRENT_LIST_DIR})
    target_compile_features(SmallRegisterRefined INTERFACE cxx_std_17)
    target_compile_options(SmallRegisterRefined INTERFACE -Werror=overflow)
endmacro()


################################################################################
# Main script
################################################################################

project(SmallRegister)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

CreateMainRefinedTarget()

set(SMALL_REGISTERS_ENABLE_TESTING OFF CACHE BOOL "Enables self-testing of the library")

if(SMALL_REGISTERS_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

