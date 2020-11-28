/**
 * @file	chaining.cpp
 * @brief	Test of chained setting and clearing during runtime.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Operations are chained", "[small_register][chain]")
{
    using RegisterUnderTest =
        small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 3>, bitfield<reg::three, 3>>;
    RegisterUnderTest reg{};

    SECTION("Simple chain")
    {
        REQUIRE(reg.set<reg::one>(0b10).set<reg::three>(0b110).clear<reg::three>(0b010)() == 0b10000100);
    }

    SECTION("Simple procedural chain")
    {
        reg.set<reg::one>(0b10);
        reg.set<reg::three>(0b110);
        reg.clear<reg::three>(0b010);
        REQUIRE(reg() == 0b10000100);
    }

    SECTION("Long chain")
    {
        reg.set<reg::one>(0b11)
            .set<reg::two>(0b010)
            .set<reg::three>(0b101)
            .clear<reg::one>(0b01)
            .clear<reg::three>(0b001)
            .set<reg::two>(0b001)
            .set<reg::three>(0b011)
            .clear<reg::two>(0b100)
            .clear<reg::three>(0b100);

        REQUIRE(reg() == 0b10011011);
    }

    SECTION("Ling procedural chain")
    {
        reg.set<reg::one>(0b11);
        reg.set<reg::two>(0b010);
        reg.set<reg::three>(0b101);
        reg.clear<reg::one>(0b01);
        reg.clear<reg::three>(0b001);
        reg.set<reg::two>(0b001);
        reg.set<reg::three>(0b011);
        reg.clear<reg::two>(0b100);
        reg.clear<reg::three>(0b100);

        REQUIRE(reg() == 0b10011011);
    }

    SECTION("Long chain with whole bitfield operations")
    {
        reg.set<reg::one>(0b11)
            .set<reg::two>(0b010)
            .set<reg::three>(0b101)
            .clear<reg::one>()
            .set<reg::one>(0b01)
            .set<reg::three>(0b011)
            .clear<reg::two>()
            .clear<reg::three>();

        REQUIRE(reg() == 0b01000000);
    }

    SECTION("Long procedural chain with whole bitfield operations")
    {
        reg.set<reg::one>(0b11);
        reg.set<reg::two>(0b010);
        reg.set<reg::three>(0b101);
        reg.clear<reg::one>();
        reg.set<reg::one>(0b01);
        reg.set<reg::three>(0b011);
        reg.clear<reg::two>();
        reg.clear<reg::three>();

        REQUIRE(reg() == 0b01000000);
    }
}
