/**
 * @file	small_map.hpp
 * @brief	Implements a small map that maps addresses to corresponding registers.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_MAP_HPP
#define SMALL_MAP_HPP

#include "small_register/small_register.hpp"
#include "small_register/small_register_internal.hpp"

#include <array>
#include <tuple>
#include <type_traits>

namespace jungles
{

/**
 * Element of the map that relates the Address to a type SmallRegister which shall be a template instance
 * of jungles::small_register.
 */
template<auto Address, typename SmallRegister>
struct element
{
    static constexpr auto address{Address};
    using Register = SmallRegister;
};

/**
 * \brief Maps values to specific jungles::small_register instances.
 * \tparam Elements Multiple jungles::element which are elements of the map.
 */
template<typename... Elements>
struct small_map
{
  private:
    static inline constexpr std::array addresses{Elements::address...};
    static inline constexpr std::tuple<typename Elements::Register...> registers = {};

  public:
    /**
     * \brief Performs the mapping at compile time. Use register_from_address::type alias to obtain the type.
     * \tparam Register address (the value) that is key to obtain the type for that register address.
     */
    template<auto Address>
    struct register_from_address
    {
      private:
        static inline constexpr auto address_it{detail::find(std::begin(addresses), std::end(addresses), Address)};
        static_assert(address_it != std::end(addresses), "Register address not found");

        static inline constexpr auto type_index{std::distance(std::begin(addresses), address_it)};

        using ElementType = typename std::tuple_element<type_index, std::tuple<Elements...>>::type;

      public:
        using type = typename ElementType::Register;
    };
};

}; // namespace jungles

#endif /* SMALL_MAP_HPP */
