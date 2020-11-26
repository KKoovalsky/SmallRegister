/**
 * @file	single_byte_clearing_whole_fields.cpp
 * @brief	Test of clearing fields in a single byte bitfield by whole fields.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>
#include <type_traits>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are cleared in one byte size bitfield", "[small_register][one_byte_size][clear]")
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

        auto r{some_reg.set(bitfield<reg::one>{1},
                            bitfield<reg::two>{1},
                            bitfield<reg::three>{1},
                            bitfield<reg::four>{1},
                            bitfield<reg::five>{1},
                            bitfield<reg::six>{1},
                            bitfield<reg::seven>{1},
                            bitfield<reg::eight>{1})};

        SECTION("Single bitfield is cleared")
        {
            REQUIRE(r.clear<reg::four>()() == 0b11101111);
        }

        SECTION("Multiple bitfields are cleared")
        {
            REQUIRE(r.clear<reg::one, reg::three, reg::six>()() == 0b01011011);
        }
    }

    SECTION("For three bitfields")
    {
        static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;
        auto r{some_reg.set(bitfield<reg::one>{0b11}, bitfield<reg::two>{0b101}, bitfield<reg::three>{0b010})};

        SECTION("First bitfield is cleared")
        {
            REQUIRE(r.clear<reg::one>()() == 0b00101010);
        }

        SECTION("Second bitfield is cleared")
        {
            REQUIRE(r.clear<reg::two>()() == 0b11000010);
        }

        SECTION("Third bitfield is cleared")
        {
            REQUIRE(r.clear<reg::three>()() == 0b11101000);
        }

        SECTION("First and third bitfield is cleared")
        {
            REQUIRE(r.clear<reg::one, reg::three>()() == 0b00101000);
        }
    }
}
