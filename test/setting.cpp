/**
 * @file	setting.cpp
 * @brief	Test whether setting of bits in a single register works.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register_refined/small_register.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are set", "[small_register][set]")
{
    SECTION("For one big bitfield")
    {
        using RegisterWithOneBigBitfield = small_register<uint8_t, bitfield<reg::one, 8>>;
        RegisterWithOneBigBitfield reg{};

        SECTION("For single setting")
        {
            SECTION("Specified bits are set")
            {
                REQUIRE(reg.set<reg::one>(0b01100000)() == 0b01100000);
            }

            SECTION("Whole bitfield is set when specified")
            {
                REQUIRE(reg.set<reg::one>(0xFF)() == 0xFF);
            }

            SECTION("Whole bitfield is set when no bit specified")
            {
                REQUIRE(reg.set<reg::one>()() == 0xFF);
            }
        }
    }

    SECTION("For two bitfields with different size")
    {
        using RegisterWithTwoBitfields = small_register<uint8_t, bitfield<reg::one, 6>, bitfield<reg::two, 2>>;
        RegisterWithTwoBitfields reg{};

        SECTION("Setting first bitfield")
        {
            SECTION("Specified bits are set")
            {
                REQUIRE(reg.set<reg::one>(0b010101)() == 0b01010100);
            }

            SECTION("Whole bitfield is set")
            {
                REQUIRE(reg.set<reg::one>()() == 0b11111100);
            }
        }

        SECTION("Setting second bitfield")
        {
            SECTION("Specified bits are set")
            {
                REQUIRE(reg.set<reg::two>(0b10)() == 0b00000010);
            }

            SECTION("Whole bitfield is set when set explicitly")
            {
                REQUIRE(reg.set<reg::two>(0b11)() == 0b11);
            }
        }
    }

    SECTION("For three bitfields")
    {
        using RegisterWithThreeBitfields =
            small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 3>, bitfield<reg::three, 3>>;
        RegisterWithThreeBitfields reg{};

        SECTION("Setting first bitfield")
        {
            SECTION("Specified bits are set")
            {
                REQUIRE(reg.set<reg::one>(0b01)() == 0b01000000);
            }

            SECTION("Whole bitfield is set")
            {
                REQUIRE(reg.set<reg::one>()() == 0b11000000);
            }

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.set<reg::one>(0b11));
                REQUIRE_THROWS(reg.set<reg::one>(0b100));
            }
        }

        SECTION("Setting second bitfield")
        {
            SECTION("Specified bits are set")
            {
                REQUIRE(reg.set<reg::two>(0b010)() == 0b00010000);
            }

            SECTION("Whole bitfield is set")
            {
                REQUIRE(reg.set<reg::two>()() == 0b00111000);
            }

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.set<reg::two>(0b111));
                REQUIRE_THROWS(reg.set<reg::two>(0b1000));
            }
        }

        SECTION("Setting third bitfield")
        {
            REQUIRE(reg.set<reg::three>(0b110)() == 0b110);

            SECTION("Overflow is detected properly")
            {
                REQUIRE_NOTHROW(reg.set<reg::three>(0b111));
                REQUIRE_THROWS(reg.set<reg::three>(0b1000));
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
        RegisterWithEightBitfields reg{};

        SECTION("First bitfield is set properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.set<reg::one>(1)() == 0b10000000);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.set<reg::one>()() == 0b10000000);
            }
        }

        SECTION("Third bitfield is set properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.set<reg::three>(1)() == 0b00100000);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.set<reg::three>()() == 0b00100000);
            }
        }

        SECTION("Eighth bitfield is set properly")
        {
            SECTION("Explicitly")
            {
                REQUIRE(reg.set<reg::eight>(1)() == 0b1);
            }
            SECTION("Implicitly")
            {
                REQUIRE(reg.set<reg::eight>()() == 0b1);
            }
        }
    }

    SECTION("Bitfields are set in chain")
    {
        using RegisterWithThreeBitfields =
            small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 4>, bitfield<reg::three, 2>>;

        RegisterWithThreeBitfields reg{};

        SECTION("By proper chain")
        {
            REQUIRE(reg.set<reg::one>().set<reg::two>(0b0101).set<reg::three>(0b10)() == 0b11010110);
        }

        SECTION("By procedural code")
        {
            reg.set<reg::one>();
            reg.set<reg::two>(0b0101);
            reg.set<reg::three>(0b10);
            REQUIRE(reg() == 0b11010110);
        }
    }

    SECTION("For two byte long register")
    {
        using RegisterWithThreeBitfields =
            small_register<uint16_t, bitfield<reg::one, 3>, bitfield<reg::two, 12>, bitfield<reg::three, 1>>;
        RegisterWithThreeBitfields reg{};

        SECTION("Second bitfield is set properly")
        {
            SECTION("When set explicitly")
            {
                REQUIRE(reg.set<reg::two>(0b010101010101)() == 0b0000101010101010);
            }

            SECTION("When set implicitly")
            {
                REQUIRE(reg.set<reg::two>()() == 0b0001111111111110);
            }
        }
    }
}
