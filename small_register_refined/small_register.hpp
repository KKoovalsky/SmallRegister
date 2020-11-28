/**
 * @file	small_register.hpp
 * @brief	Small register definition and implementation.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_REGISTER_HPP
#define SMALL_REGISTER_HPP

#include <stdexcept>

#include "small_register_refined/small_register_internal.hpp"

namespace jungles
{

template<auto Id, unsigned Size>
struct bitfield
{
    static inline constexpr auto id{Id};
    static inline constexpr auto size{Size};
};

template<typename RegisterUnderlyingType, typename... Bitfields>
class small_register
{
  private:
    using AreTypesOfIdTheSame = detail::are_same<decltype(Bitfields::id)...>;

    // TODO cmake test for that
    static_assert(AreTypesOfIdTheSame::value, "bitfield::id types shall be the same");

    using Register = RegisterUnderlyingType;
    using Self = small_register<RegisterUnderlyingType, Bitfields...>;

    static constexpr unsigned bit_size{sizeof(Register) * 8};

    static inline constexpr std::array ids{Bitfields::id...};
    static inline constexpr std::array sizes{Bitfields::size...};
    static inline constexpr unsigned accumulated_size{detail::accumulate(std::begin(sizes), std::end(sizes), 0)};

    static_assert(accumulated_size == bit_size, "Accumulated bit-size must be 8");
    static_assert(detail::has_unique(std::begin(ids), std::end(ids)), "Bitfield IDs must be unique");

    template<auto Id>
    static inline constexpr auto find_index()
    {
        constexpr auto it{detail::find(std::begin(ids), std::end(ids), Id)};
        static_assert(it != std::end(ids), "Bitfield ID not found");
        return std::distance(std::begin(ids), it);
    }

    template<auto Id>
    static inline constexpr auto find_corresponding_size()
    {
        constexpr auto index{find_index<Id>()};
        return std::next(std::begin(sizes), index);
    }

    template<auto Id>
    static inline constexpr auto find_shift()
    {
        constexpr auto size_it{find_corresponding_size<Id>()};
        return detail::accumulate(std::next(size_it, 1), std::end(sizes), 0);
    }

    template<auto Id>
    static inline constexpr unsigned get_maximum_value()
    {
        auto sizes_it{find_corresponding_size<Id>()};
        auto bitsize{*sizes_it};
        return (1 << bitsize) - 1;
    }

  public:
    small_register(Register initial_value = 0) : underlying_register{initial_value}
    {
    }

    template<auto Id>
    constexpr inline Self& set()
    {
        constexpr auto value{get_maximum_value<Id>()};
        return set<Id>(value);
    }

    template<auto Id>
    constexpr inline Self& set(unsigned value)
    {
        constexpr auto max_value{get_maximum_value<Id>()};
        if (value > max_value)
            throw overflow_error{};

        constexpr auto shift{find_shift<Id>()};
        underlying_register |= (value << shift);
        return *this;
    }

    template<auto Id>
    constexpr inline unsigned get()
    {
        constexpr auto mask{get_maximum_value<Id>()};
        constexpr auto shift{find_shift<Id>()};
        return (underlying_register >> shift) & mask;
    }

    template<auto Id>
    constexpr inline Self& clear()
    {
        constexpr auto mask{get_maximum_value<Id>()};
        return clear<Id>(mask);
    }

    template<auto Id>
    constexpr inline Self& clear(unsigned mask)
    {
        constexpr auto strongest_mask{get_maximum_value<Id>()};
        if (mask > strongest_mask)
            throw mask_not_matching_error{};

        constexpr auto shift{find_shift<Id>()};
        underlying_register &= ~(mask << shift);
        return *this;
    }

    constexpr RegisterUnderlyingType operator()() const
    {
        return underlying_register;
    }

    struct mask_not_matching_error : std::exception
    {
    };

    struct overflow_error : std::exception
    {
    };

  private:
    RegisterUnderlyingType underlying_register;
};

} // namespace jungles

#endif /* SMALL_REGISTER_HPP */
