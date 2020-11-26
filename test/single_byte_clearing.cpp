/**
 * @file	single_byte_clearing.cpp
 * @brief	Test of clearing fields in a single byte bitfield.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>
#include <type_traits>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Individual bits are cleared in one byte size bitfield", "[small_register][one_byte_size][clear]")
{
    SECTION("Non matching mask is detected correctly")
    {
        SECTION("For 8-bit size bitfield")
        {
            static constexpr small_register<bits<reg::one, 8>> some_reg;
            REQUIRE_THROWS(some_reg.set().clear_individual_bits(bitfield<reg::one>{0x1FF}));
            REQUIRE_NOTHROW(some_reg.set().clear_individual_bits(bitfield<reg::one>{0xFF}));
        }

        SECTION("For 1-bit size bitfield")
        {
            static constexpr small_register<bits<reg::one, 1>, bits<reg::two, 7>> some_reg;
            REQUIRE_THROWS(some_reg.set().clear_individual_bits(bitfield<reg::one>{0b10}));
            REQUIRE_NOTHROW(some_reg.set().clear_individual_bits(bitfield<reg::one>{0b01}));
        }

        SECTION("For 3-bit size bitfield")
        {
            static constexpr small_register<bits<reg::one, 3>, bits<reg::two, 5>> some_reg;
            REQUIRE_THROWS(some_reg.set().clear_individual_bits(bitfield<reg::one>{0b1000}));
            REQUIRE_NOTHROW(some_reg.set().clear_individual_bits(bitfield<reg::one>{0b111}));
        }
    }

    SECTION("Bits are cleared for individual bits")
    {
        SECTION("For one bit-size field")
        {
            static constexpr small_register<bits<reg::one, 1>, bits<reg::two, 7>> some_reg;
            REQUIRE(some_reg.set(bitfield<reg::one>{1}, bitfield<reg::two>{0b010})
                        .clear_individual_bits(bitfield<reg::one>{1})()
                    == 0b010);
        }

        SECTION("For two bit-size field")
        {
            static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 6>> some_reg;
            auto r{some_reg.set(bitfield<reg::one>{0b11})};

            SECTION("One bit is cleared")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b10})() == 0b01000000);
            }

            SECTION("Two bits are cleared")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b11})() == 0b00000000);
            }
        }

        SECTION("For five bit-size fields")
        {
            static constexpr small_register<bits<reg::one, 5>, bits<reg::two, 3>> some_reg;
            auto r{some_reg.set(bitfield<reg::one>{0b11111})};

            SECTION("One bit is cleared")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b00100})() == 0b11011000);
            }

            SECTION("All bits are cleared")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b11111})() == 0);
            }

            SECTION("Three bits are cleared")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b11111})() == 0);
            }
        }
    }

    SECTION("Bits are cleared for bitfields in various places")
    {
        SECTION("When all bitfields are of size 1")
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
            REQUIRE(r.clear_individual_bits(bitfield<reg::two>{1}, bitfield<reg::five>{1}, bitfield<reg::eight>{1})()
                    == 0b10110110);
        }

        SECTION("For three bitfields")
        {
            static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;
            auto r{some_reg.set(bitfield<reg::one>{0b11}, bitfield<reg::two>{0b111}, bitfield<reg::three>{0b111})};

            SECTION("When first bitfield is changed")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::one>{0b01})() == 0b10111111);
            }

            SECTION("When second bitfield is changed")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::two>{0b110})() == 0b11001111);
            }

            SECTION("When third bitfield is changed")
            {
                REQUIRE(r.clear_individual_bits(bitfield<reg::three>{0b011})() == 0b11111100);
            }
        }
    }
}
