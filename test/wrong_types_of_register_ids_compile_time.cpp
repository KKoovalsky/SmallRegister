/**
 * @file	wrong_types_of_register_ids_compile_time.cpp
 * @brief	Input for a test that checks whether static assertion fails when register IDs have various types.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "small_register/small_map.hpp"

using namespace jungles;

enum class reg1
{
    one,
    two
};

enum class reg2
{
    one,
    two
};

void wrong_types_of_register_ids_compile_time()
{
    using Reg1 = small_register<uint8_t, bitfield<reg1::one, 2>, bitfield<reg2::two, 6>>;

    Reg1{};
}

