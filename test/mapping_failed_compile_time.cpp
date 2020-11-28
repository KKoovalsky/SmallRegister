/**
 * @file	mapping_failed_compile_time.cpp
 * @brief	Test for static assertion is triggered on wrong register address supplied.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "small_register/small_map.hpp"

#include "helpers.hpp"

using namespace jungles;

void mapping_failed_compile_time()
{
    using Reg1 = small_register<uint8_t, bitfield<reg::one, 2>, bitfield<reg::two, 6>>;
    using Reg2 = small_register<bitfield<reg::three, 4>, bitfield<reg::four, 4>>;

    using MemoryMap = small_map<element<0x01, Reg1>, element<0x02, Reg2>>;

    MemoryMap::register_from_address<0x03>::type{};
}

