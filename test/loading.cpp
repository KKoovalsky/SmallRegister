/**
 * @file	loading.cpp
 * @brief	Tests creation from register obtained over a wire, from external source, etc.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register_refined/small_register.hpp"

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Small register content is loaded", "[small_register][one_byte_size][from_raw]")
{
    SECTION("For 1-bit size bitfields")
    {
        using OneBitSizedBitfieldsRegister = small_register<uint8_t,
                                                            bitfield<reg::one, 1>,
                                                            bitfield<reg::two, 1>,
                                                            bitfield<reg::three, 1>,
                                                            bitfield<reg::four, 1>,
                                                            bitfield<reg::five, 1>,
                                                            bitfield<reg::six, 1>,
                                                            bitfield<reg::seven, 1>,
                                                            bitfield<reg::eight, 1>>;

        SECTION("Zero is loaded by default")
        {
            OneBitSizedBitfieldsRegister reg{};

            REQUIRE(reg.get<reg::one>() == 0);
            REQUIRE(reg.get<reg::two>() == 0);
            REQUIRE(reg.get<reg::three>() == 0);
            REQUIRE(reg.get<reg::four>() == 0);
            REQUIRE(reg.get<reg::five>() == 0);
            REQUIRE(reg.get<reg::six>() == 0);
            REQUIRE(reg.get<reg::seven>() == 0);
            REQUIRE(reg.get<reg::eight>() == 0);
        }

        SECTION("Preset value is loaded")
        {
            OneBitSizedBitfieldsRegister reg{0b00110101};

            REQUIRE(reg.get<reg::one>() == 0);
            REQUIRE(reg.get<reg::two>() == 0);
            REQUIRE(reg.get<reg::three>() == 1);
            REQUIRE(reg.get<reg::four>() == 1);
            REQUIRE(reg.get<reg::five>() == 0);
            REQUIRE(reg.get<reg::six>() == 1);
            REQUIRE(reg.get<reg::seven>() == 0);
            REQUIRE(reg.get<reg::eight>() == 1);
        }
    }

    SECTION("For three bitfields")
    {
        using ThreeBitfieldsInRegister =
            small_register<uint16_t, bitfield<reg::one, 4>, bitfield<reg::two, 5>, bitfield<reg::three, 7>>;

        ThreeBitfieldsInRegister reg{0b1001110010110101};

        REQUIRE(reg.get<reg::one>() == 0b1001);
        REQUIRE(reg.get<reg::two>() == 0b11001);
        REQUIRE(reg.get<reg::three>() == 0b110101);
    }
}
