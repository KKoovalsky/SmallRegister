/**
 * @file	abbreviated_forms.cpp
 * @brief	Tests abbreviated forms.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "small_register/abbreviated_forms.hpp"
#include "catch2/catch_test_macros.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Abbreviated forms work well", "[small_register][abbreviated_forms]")
{
    static constexpr small_register<b<reg::one, 1>,
                                    b<reg::two, 1>,
                                    b<reg::three, 1>,
                                    b<reg::four, 1>,
                                    b<reg::five, 1>,
                                    b<reg::six, 1>,
                                    b<reg::seven, 1>,
                                    b<reg::eight, 1>>
        some_reg;

    REQUIRE(some_reg.set(bf<reg::one>{1}, bf<reg::three>{1})() == 0b10100000);
}
