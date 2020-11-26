/**
 * @file	small_register.hpp
 * @brief	Small register implementation mostly for mapping registers in sensors accessile through I2C, SPI, etc.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_REGISTER_HPP
#define SMALL_REGISTER_HPP

#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

namespace jungles
{

namespace detail
{
template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            return first;
        }
    }
    return last;
}

template<class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first)
    {
        init = std::move(init) + *first;
    }
    return init;
}

template<typename InputIt>
constexpr bool has_unique(InputIt first, InputIt last)
{
    for (; first != last; ++first)
    {
        for (auto it{first + 1}; it != last; ++it)
        {
            if (*it == *first)
                return false;
        }
    }
    return true;
}

} // namespace detail

template<auto EnumVal, std::size_t Size>
struct bits
{
    static constexpr auto name{EnumVal};
    static constexpr auto size{Size};
};

template<auto Name>
struct bitfield
{
    static constexpr auto name{Name};

    constexpr bitfield(unsigned value) : value{value}
    {
    }

    const unsigned value{0};
};

template<typename... Bits>
class small_register
{
  public:
    using Register = unsigned char;

  private:
    static constexpr unsigned bit_size{sizeof(Register) * 8};

    static inline constexpr std::array names{Bits::name...};
    static inline constexpr std::array sizes{Bits::size...};
    static inline constexpr unsigned accumulated_size{detail::accumulate(std::begin(sizes), std::end(sizes), 0)};

    using NameType = typename std::remove_reference<decltype(names)>::type;

    static_assert(accumulated_size == bit_size, "Accumulated bit-size must be 8");
    static_assert(detail::has_unique(std::begin(names), std::end(names)), "Bitfield names must be unique");

    template<auto BitfieldName>
    static inline constexpr auto find_index()
    {
        constexpr auto it{detail::find(std::begin(names), std::end(names), BitfieldName)};
        static_assert(it != std::end(names), "Bitfield name not found");
        return std::distance(std::begin(names), it);
    }

    template<auto BitfieldName>
    static inline constexpr auto to_sizes_iterator()
    {
        constexpr auto index{find_index<BitfieldName>()};
        return std::next(std::begin(sizes), index);
    }

    template<auto BitfieldName>
    static inline constexpr auto find_shift()
    {
        constexpr auto sizes_it{to_sizes_iterator<BitfieldName>()};
        return detail::accumulate(std::next(sizes_it, 1), std::end(sizes), 0);
    }

    template<auto BitfieldName>
    static inline constexpr unsigned get_maximum_value()
    {
        auto sizes_it{to_sizes_iterator<BitfieldName>()};
        auto bitsize{*sizes_it};
        return (1 << bitsize) - 1;
    }

  public:
    template<auto BitfieldName>
    static inline constexpr bitfield<BitfieldName> create_bitfield_with_all_bits_set()
    {
        constexpr auto all_bits_set{get_maximum_value<BitfieldName>()};
        return bitfield<BitfieldName>{all_bits_set};
    }

  private:
    class proxy
    {
      public:
        constexpr proxy(Register v) : underlying_value{v}
        {
        }

        Register operator()() const
        {
            return underlying_value;
        }

        template<typename... BitfieldValues>
        inline proxy& set(BitfieldValues... bitfield_values)
        {
            auto v{small_register<Bits...>::set(bitfield_values...)};
            underlying_value |= v();
            return *this;
        }

        template<typename... BitfieldValues>
        inline proxy& clear_individual_bits(BitfieldValues... bitfield_masks)
        {
            auto mask{small_register<Bits...>::get_clearing_mask(bitfield_masks...)};
            underlying_value &= mask();
            return *this;
        }

        template<auto... BitfieldNames>
        inline proxy& clear()
        {
            clear_individual_bits(small_register<Bits...>::create_bitfield_with_all_bits_set<BitfieldNames>()...);
            return *this;
        }

      private:
        Register underlying_value = {};
    };

  public:
    template<auto BitfieldName>
    static inline constexpr Register set_bitfield(bitfield<BitfieldName> bitfield_value)
    {
        constexpr auto max_value{get_maximum_value<BitfieldName>()};
        auto value{bitfield_value.value};
        if (value > max_value)
            throw overflow_error{};

        constexpr auto shift{find_shift<BitfieldName>()};
        return value << shift;
    }

    template<typename... BitfieldValues>
    static constexpr inline proxy set(BitfieldValues... bitfield_values)
    {
        return (set_bitfield(bitfield_values) | ... | 0);
    }

    template<auto BitfieldName>
    static inline constexpr Register get_bitfield_clearing_mask(bitfield<BitfieldName> bitfield_mask)
    {
        constexpr auto max_value{get_maximum_value<BitfieldName>()};
        auto value{bitfield_mask.value};
        if (value > max_value)
            throw mask_not_matching_error{};

        return ~set_bitfield(bitfield_mask);
    }

    template<typename... BitfieldValues>
    static constexpr inline proxy get_clearing_mask(BitfieldValues... bitfield_values)
    {
        constexpr auto ones_only{~Register{}};
        return (get_bitfield_clearing_mask(bitfield_values) & ... & ones_only);
    }

    struct overflow_error : std::exception
    {
    };

    struct mask_not_matching_error : std::exception
    {
    };
};

} // namespace jungles

#endif /* SMALL_REGISTER_HPP */
