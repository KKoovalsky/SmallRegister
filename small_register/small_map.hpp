/**
 * @file	small_map.hpp
 * @brief	Implements a small map that maps addresses to corresponding registers.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_MAP_HPP
#define SMALL_MAP_HPP

#include "small_register/small_details.hpp"
#include "small_register/small_register.hpp"

#include <array>
#include <stdexcept>
#include <tuple>

namespace jungles
{

template<uint8_t Address, typename SmallRegister>
struct element
{
    static constexpr auto address{Address};
    using Register = SmallRegister;
};

template<typename... Elements>
struct small_map
{
  private:
    static inline constexpr std::array addresses{Elements::address...};
    static inline constexpr std::tuple<typename Elements::Register...> registers = {};

  public:
    template<auto Address>
    static inline constexpr auto get_register()
    {
        constexpr auto address_it{detail::find(std::begin(addresses), std::end(addresses), Address)};
        static_assert(address_it != std::end(addresses), "Register address not found");
        constexpr auto index{std::distance(std::begin(addresses), address_it)};
        return std::get<index>(registers);
    }
};

}; // namespace jungles

#endif /* SMALL_MAP_HPP */
