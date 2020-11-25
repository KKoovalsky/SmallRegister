/**
 * @file	single_byte_setting.cpp
 * @brief	Test of setting fields in a single byte bitfield.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>
#include <type_traits>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are set in one byte size bitfield", "[small_register][one_byte_size]")
{
    SECTION("Register contains one big bitfield")
    {
        static constexpr small_register<bits<reg::one, 8>> some_reg;

        SECTION("Bits are set correctly")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{76}) == 76);
        }

        SECTION("Zero is obtained when no bits set")
        {
            REQUIRE(some_reg.set() == 0);
        }

        SECTION("Overflow is detected")
        {
            REQUIRE_THROWS(some_reg.set(bitfield<reg::one>{256}));
        }
    }

    SECTION("Register contains two bitfields with different size")
    {
        static constexpr small_register<bits<reg::one, 3>, bits<reg::two, 5>> some_reg;

        SECTION("Only first field is set")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b11}) == 0b01100000);
        }

        SECTION("Only second field is set")
        {
            REQUIRE(some_reg.set(bitfield<reg::two>{0b00110}) == 0b00000110);
        }

        SECTION("Both fields are set")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b010}, bitfield<reg::two>{0b01010}) == 0b01001010);
        }
    }

    SECTION("Register contains three bitfields")
    {
        static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;

        SECTION("All bitfields are set")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::two>{0b010}, bitfield<reg::three>{0b100})
                    == 0b10010100);
        }
    }

    SECTION("Setting to zeros doesn't harm")
    {
        static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;

        SECTION("All bitfields are set")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::two>{0}, bitfield<reg::three>{0b100})
                    == 0b10000100);
        }
    }

    SECTION("Register contains eight bitfields")
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

        SECTION("Few are set")
        {
            REQUIRE(some_reg.set(bitfield<reg::two>{1}, bitfield<reg::five>{1}, bitfield<reg::seven>{1}) == 0b01001010);
        }
    }

    SECTION("Overflows are detected")
    {
        SECTION("For bitfield of length 1")
        {
            static constexpr small_register<bits<reg::one, 1>, bits<reg::two, 7>> some_reg;
            REQUIRE_THROWS(some_reg.set(bitfield<reg::one>{2}));
        }

        SECTION("For bitfield of length 2")
        {
            static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 6>> some_reg;
            REQUIRE_THROWS(some_reg.set(bitfield<reg::one>{4}));
        }

        SECTION("For bitfield of length 6")
        {
            static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 6>> some_reg;
            REQUIRE_THROWS(some_reg.set(bitfield<reg::two>{0b1000000}));
        }
    }
}
