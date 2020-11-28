/**
 * @file	mapping.cpp
 * @brief	Mapping works find.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register_refined/small_map.hpp"

#include "helpers.hpp"

#include <boost/type_index.hpp>

using namespace jungles;

enum class reg1
{
    bitfield1,
    bitfield2,
    bitfield3,
    bitfield4,
};

enum class reg2
{
    bitfield1,
    bitfield2,
    bitfield3
};

TEST_CASE("Mapping of registers can be performed", "[small_register][small_map]")
{
    SECTION("Access map with two registers")
    {
        using Reg1 = small_register<uint8_t,
                                    bitfield<reg1::bitfield1, 2>,
                                    bitfield<reg1::bitfield2, 3>,
                                    bitfield<reg1::bitfield3, 3>>;
        using Reg2 = small_register<uint8_t,
                                    bitfield<reg2::bitfield1, 2>,
                                    bitfield<reg2::bitfield2, 4>,
                                    bitfield<reg2::bitfield3, 2>>;

        using MemoryMap = small_map<element<0x01, Reg1>, element<0x02, Reg2>>;

        REQUIRE(std::is_same_v<MemoryMap::register_from_address<0x01>::type, Reg1>);
        REQUIRE(std::is_same_v<MemoryMap::register_from_address<0x02>::type, Reg2>);
    }
}
