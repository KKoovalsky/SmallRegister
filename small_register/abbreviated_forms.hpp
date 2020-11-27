/**
 * @file	abbreviated_forms.hpp
 * @brief	Abbreviated forms for small_register API
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef ABBREVIATED_FORMS_HPP
#define ABBREVIATED_FORMS_HPP

#include "small_register/small_map.hpp"
#include "small_register/small_register.hpp"

namespace jungles
{
template<auto EnumVal, std::size_t Size>
using b = struct bits<EnumVal, Size>;

template<auto Name>
using bf = struct bitfield<Name>;

template<uint8_t Address, typename SmallRegister>
using e = struct element<Address, SmallRegister>;

}; // namespace jungles

#endif /* ABBREVIATED_FORMS_HPP */
