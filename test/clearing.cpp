/**
 * @file	clearing.cpp
 * @brief	Tests whether clearing works well.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are cleared", "[small_register][clear]")
{
    SECTION("For one big bitfield")
    {
        using RegisterWithOneBigBitfield = small_register<uint8_t, bitfield<reg::one, 8>>;
        RegisterWithOneBigBitfield reg{0xFF};

        SECTION("Specified bits are cleared")
        {
            REQUIRE(reg.clear<reg::one>(0b01100000)() == 0b10011111);
        }

        SECTION("Whole bitfield is cleared when specified")
        {
            REQUIRE(reg.clear<reg::one>(0xFF)() == 0);
        }

        SECTION("Whole bitfield is cleared when no bit specified")
        {
            REQUIRE(reg.clear<reg::one>()() == 0);
        }
    }

    SECTION("For two bitfields with different size")
    {
        using RegisterWithTwoBitfields = small_register<uint8_t, bitfield<reg::one, 6>, bitfield<reg::two, 2>>;
        RegisterWithTwoBitfields reg{0b10101010};

        SECTION("Clearing first bitfield")
        {
            SECTION("Specified bits are cleared")
            {
                REQUIRE(reg.clear<reg::one>(0b100010)() == 0b00100010);
            }

            SECTION("Whole bitfield is cleared")
            {
                REQUIRE(reg.clear<reg::one>()() == 0b10);
            }
        }

        SECTION("Clearing second bitfield")
        {
            SECTION("Specified bits are cleared")
            {
                REQUIRE(reg.clear<reg::two>(0b10)() == 0b10101000);
            }

            SECTION("Whole bitfield is cleared when cleared explicitly")
            {
                REQUIRE(reg.clear<reg::two>(0b11)() == 0b10101000);
            }
        }
    }

    SECTION("For three bitfields")
    {
        using RegisterWithThreeBitfields =
            small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 3>, bitfield<reg::three, 3>>;
        RegisterWithThreeBitfields reg{0xFF};

        SECTION("Clearing first bitfield")
        {
            SECTION("Specified bits are cleared")
            {
                REQUIRE(reg.clear<reg::one>(0b01)() == 0b10111111);
            }

            SECTION("Whole bitfield is cleared")
            {
                REQUIRE(reg.clear<reg::one>()() == 0b00111111);
            }

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.clear<reg::one>(0b11));
                REQUIRE_THROWS(reg.clear<reg::one>(0b100));
            }
        }

        SECTION("Clearing second bitfield")
        {
            SECTION("Specified bits are cleared")
            {
                REQUIRE(reg.clear<reg::two>(0b010)() == 0b11101111);
            }

            SECTION("Whole bitfield is cleared")
            {
                REQUIRE(reg.clear<reg::two>()() == 0b11000111);
            }

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.clear<reg::two>(0b111));
                REQUIRE_THROWS(reg.clear<reg::two>(0b1000));
            }
        }

        SECTION("Clearing third bitfield")
        {
            REQUIRE(reg.clear<reg::three>(0b110)() == 0b11111001);

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.clear<reg::three>(0b111));
                REQUIRE_THROWS(reg.clear<reg::three>(0b1000));
            }
        }
    }

    SECTION("For single-byte bitfields")
    {
        using RegisterWithEightBitfields = small_register<uint8_t,
                                                          bitfield<reg::one, 1>,
                                                          bitfield<reg::two, 1>,
                                                          bitfield<reg::three, 1>,
                                                          bitfield<reg::four, 1>,
                                                          bitfield<reg::five, 1>,
                                                          bitfield<reg::six, 1>,
                                                          bitfield<reg::seven, 1>,
                                                          bitfield<reg::eight, 1>>;
        RegisterWithEightBitfields reg{0xFF};

        SECTION("First bitfield is cleared properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.clear<reg::one>(1)() == 0b01111111);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.clear<reg::one>()() == 0b01111111);
            }
        }

        SECTION("Third bitfield is cleared properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.clear<reg::three>(1)() == 0b11011111);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.clear<reg::three>()() == 0b11011111);
            }
        }

        SECTION("Eighth bitfield is cleared properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.clear<reg::eight>(1)() == 0b11111110);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.clear<reg::eight>()() == 0b11111110);
            }
        }
    }

    SECTION("Bitfields are cleared in chain")
    {
        using RegisterWithThreeBitfields =
            small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 4>, bitfield<reg::three, 2>>;

        RegisterWithThreeBitfields reg{0xFF};

        SECTION("By proper chain")
        {
            REQUIRE(reg.clear<reg::one>().clear<reg::two>(0b0101).clear<reg::three>(0b10)() == 0b00101001);
        }

        SECTION("By procedural code")
        {
            reg.clear<reg::one>();
            reg.clear<reg::two>(0b0101);
            reg.clear<reg::three>(0b10);
            REQUIRE(reg() == 0b00101001);
        }
    }
}
