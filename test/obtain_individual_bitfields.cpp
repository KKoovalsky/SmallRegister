/**
 * @file	obtain_individual_bitfields.cpp
 * @brief	Obtaining individual bitfields
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Individual bitfields are obtained", "[small_register][one_byte_size][clear]")
{
    SECTION("When all bits are of size 1")
    {
        auto reg{small_register<bits<reg::one, 1>,
                                bits<reg::two, 1>,
                                bits<reg::three, 1>,
                                bits<reg::four, 1>,
                                bits<reg::five, 1>,
                                bits<reg::six, 1>,
                                bits<reg::seven, 1>,
                                bits<reg::eight, 1>>{}
                     .set(bitfield<reg::one>{1},
                          bitfield<reg::two>{0},
                          bitfield<reg::three>{1},
                          bitfield<reg::four>{1},
                          bitfield<reg::five>{1},
                          bitfield<reg::six>{0},
                          bitfield<reg::seven>{1},
                          bitfield<reg::eight>{1})};

        SECTION("First bitfield is obtained")
        {
            auto before{reg.get<reg::one>()};
            reg.clear<reg::one>();
            auto after{reg.get<reg::one>()};
            REQUIRE(before == 1);
            REQUIRE(after == 0);
        }

        SECTION("Bitfield in the middle is obtained")
        {
            auto before{reg.get<reg::six>()};
            reg.set(bitfield<reg::six>{1});
            auto after{reg.get<reg::six>()};
            REQUIRE(before == 0);
            REQUIRE(after == 1);
        }

        SECTION("Last bitfield is obtained")
        {
            auto before{reg.get<reg::eight>()};
            reg.clear<reg::eight>();
            auto after{reg.get<reg::eight>()};
            REQUIRE(before == 1);
            REQUIRE(after == 0);
        }
    }

    SECTION("For three bitfields")
    {
        auto reg{small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>>{}.set(
            bitfield<reg::one>{0b11}, bitfield<reg::two>{0b101}, bitfield<reg::three>{0b010})};

        SECTION("First bitfield is obtained")
        {
            REQUIRE(reg.get<reg::one>() == 0b11);
        }

        SECTION("Second bitfield is obtained")
        {
            REQUIRE(reg.get<reg::two>() == 0b101);
        }

        SECTION("Third bitfield is obtained")
        {
            REQUIRE(reg.get<reg::three>() == 0b010);
        }
    }
}
