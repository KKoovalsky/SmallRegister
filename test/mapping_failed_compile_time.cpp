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
    using Reg1 = small_register<bits<reg::one, 2>, bits<reg::two, 6>>;
    using Reg2 = small_register<bits<reg::three, 4>, bits<reg::four, 4>>;

    static constexpr small_map<element<0x01, Reg1>, element<0x02, Reg2>> some_map;

    static constexpr auto r1{some_map.get_register<0x03>()};
}

