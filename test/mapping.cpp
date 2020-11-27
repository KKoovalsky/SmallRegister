/**
 * @file	mapping.cpp
 * @brief	Mapping works find.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_map.hpp"

#include "helpers.hpp"

using namespace jungles;

enum class reg1
{
    bits1,
    bits2,
    bits3
};

enum class reg2
{
    bits1,
    bits2,
    bits3
};

TEST_CASE("Mapping of registers can be performed", "[small_register][small_map]")
{
    SECTION("Simple map with two registers")
    {
        using Reg1 = small_register<bits<reg1::bits1, 2>, bits<reg1::bits2, 3>, bits<reg1::bits3, 3>>;
        using Reg2 = small_register<bits<reg2::bits1, 2>, bits<reg2::bits2, 4>, bits<reg2::bits3, 2>>;

        static constexpr small_map<element<0x01, Reg1>, element<0x02, Reg2>> some_map;

        static constexpr auto r1{some_map.get_register<0x01>()};
        static constexpr auto r2{some_map.get_register<0x02>()};

        REQUIRE(r1.set(bitfield<reg1::bits2>{0b111})() == 0b00111000);
        REQUIRE(r2.set(bitfield<reg2::bits2>{0b1111})() == 0b00111100);
    }
}
