/**
 * @file	created_from_existing_raw_register.cpp
 * @brief	Tests creation from register obtained over a wire, from external source.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Small register is created from a raw register", "[small_register][one_byte_size][from_raw]")
{
    SECTION("For 1-bit size bitfields")
    {
        static constexpr small_register<bits<reg::one, 1>,
                                        bits<reg::two, 1>,
                                        bits<reg::three, 1>,
                                        bits<reg::four, 1>,
                                        bits<reg::five, 1>,
                                        bits<reg::six, 1>,
                                        bits<reg::seven, 1>,
                                        bits<reg::eight, 1>>
            some_reg;

        constexpr auto reg{some_reg.from_raw(0b00110101)};

        REQUIRE(reg.get<reg::one>() == 0);
        REQUIRE(reg.get<reg::two>() == 0);
        REQUIRE(reg.get<reg::three>() == 1);
        REQUIRE(reg.get<reg::four>() == 1);
        REQUIRE(reg.get<reg::five>() == 0);
        REQUIRE(reg.get<reg::six>() == 1);
        REQUIRE(reg.get<reg::seven>() == 0);
        REQUIRE(reg.get<reg::eight>() == 1);
    }

    SECTION("For three bitfields")
    {
        static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;

        auto reg{some_reg.from_raw(0b10011100)};

        REQUIRE(reg.get<reg::one>() == 0b10);
        REQUIRE(reg.get<reg::two>() == 0b011);
        REQUIRE(reg.get<reg::three>() == 0b100);
    }
}
