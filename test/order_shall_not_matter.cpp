/**
 * @file	order_shall_not_matter.cpp
 * @brief	Test that checks whether the order for bitfields passed to a function doesn't affect operation.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>
#include <type_traits>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are cleared and set in various order", "[small_register][one_byte_size][clear]")
{
    auto reg{small_register<bits<reg::one, 1>, bits<reg::two, 3>, bits<reg::three, 4>>{}.set(
        bitfield<reg::one>{0b1}, bitfield<reg::three>{0b0101})};

    SECTION("For set operation")
    {
        REQUIRE(reg.set(bitfield<reg::three>{0b1010}, bitfield<reg::two>{0b1})() == 0b10011111);
    }

    SECTION("For clear individual bits")
    {
        REQUIRE(reg.clear_individual_bits(bitfield<reg::three>{0b01}, bitfield<reg::one>{0b1})() == 0b00000100);
    }

    SECTION("For clear operation")
    {
        REQUIRE(reg.clear<reg::three, reg::one>()() == 0);
    }
}
