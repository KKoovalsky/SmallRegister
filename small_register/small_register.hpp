/**
 * @file	small_register.hpp
 * @brief	Small register definition and implementation.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_REGISTER_HPP
#define SMALL_REGISTER_HPP

#include <stdexcept>

#include "small_register/small_register_internal.hpp"

namespace jungles
{

/**
 * \brief Describes a bitfield.
 * \note Must be used as an input to jungles:small_register template instantiation.
 * \tparam Id Should be an ID which is a number or enumeration.
 * \tparam Size Bit-size of the bitfield.
 */
template<auto Id, unsigned Size>
struct bitfield
{
    static inline constexpr auto id{Id};
    static inline constexpr auto size{Size};
};

/**
 * \brief Simplifies bitfield handling and adds safe checks. Bitfields are in Big Endian order.
 * \tparam RegisterUnderlyingType Underlying type of the register, which determines its size.
 *                                Use e.g. uint8_t, uint16_t, uint32_t ...
 * \tparam Bitfields jungles::bitfield template instances that describe the layout of the register.
 *
 * \note There are a few static assertions performed when instantiating the template:
 * - Bitfield IDs shall be unique. Compiler raises "Bitfield IDs must be unique" otherwise.
 * - The bitfields shall give in total the bitsize of the RegisterUnderlyingType bitsize. Otherwise compiler
 *   raises "Whole register must be allocated" error. If there are unused bits then use e.g.
 *   bitfield<reg1::unused, 3>
 * - The types of bitfield IDs shall be the same. So using various "enum class" types will not work. Otherwise
 *   compiler raises "bitfield::id types shall be the same".
 * - When using methods a template parameter which is the bitfield ID is needed. When specyfying wrong ID (one which
 *   was not registered through template instantiation then a compiler error is thrown: "Bitfield ID not found"
 *
 * All the mutating methods like set(), clear(), ... return reference to self so the mutating opperations can
 * be chained: "r.set<id1>().clear<id2>().set<id3>()"
 */
template<typename RegisterUnderlyingType, typename... Bitfields>
class small_register
{
  private:
    using AreTypesOfIdTheSame = detail::are_same<decltype(Bitfields::id)...>;

    static_assert(AreTypesOfIdTheSame::value, "bitfield::id types shall be the same");

    using Register = RegisterUnderlyingType;
    using Self = small_register<RegisterUnderlyingType, Bitfields...>;

    static constexpr unsigned bit_size{sizeof(Register) * 8};

    static inline constexpr std::array ids{Bitfields::id...};
    static inline constexpr std::array sizes{Bitfields::size...};
    static inline constexpr unsigned accumulated_size{detail::accumulate(std::begin(sizes), std::end(sizes), 0)};

    static_assert(accumulated_size == bit_size, "Whole register must be allocated");
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
    static inline constexpr Register get_maximum_value()
    {
        auto sizes_it{find_corresponding_size<Id>()};
        auto bitsize{*sizes_it};
        return (1 << bitsize) - 1;
    }

  public:
    /**
     * Constructs the register with initial_value that is mapped to the defined bitfields. Initial value is zero
     * if not specified.
     */
    constexpr small_register(Register initial_value = 0) : underlying_register{initial_value}
    {
    }

    //! Sets all the bits of the bitfield to ones.
    template<auto Id>
    constexpr inline Self& set()
    {
        constexpr auto value{get_maximum_value<Id>()};
        return set<Id>(value);
    }

    /**
     * \brief Sets the bitfield to a specified value, that is equivalent to "|= value" operation on the bitfield.
     * \throws overflow_error when value is bigger than the maximum value the bitfield can store.
     */
    template<auto Id>
    constexpr inline Self& set(RegisterUnderlyingType value)
    {
        constexpr auto max_value{get_maximum_value<Id>()};
        if (value > max_value)
            throw overflow_error{};

        constexpr auto shift{find_shift<Id>()};
        underlying_register |= (value << shift);
        return *this;
    }

    //! Returns the value of the specified bitfield.
    template<auto Id>
    constexpr inline RegisterUnderlyingType get()
    {
        constexpr auto mask{get_maximum_value<Id>()};
        constexpr auto shift{find_shift<Id>()};
        return (underlying_register >> shift) & mask;
    }

    //! Clears the whole bitfield (sets all bits to zeros).
    template<auto Id>
    constexpr inline Self& clear()
    {
        constexpr auto mask{get_maximum_value<Id>()};
        return clear<Id>(mask);
    }

    /**
     * \brief Clears the bitfield applying the mask. That is equivalent to "&= ~(mask)" operation on the bitfield.
     * \throws mask_not_matching_error when mask is bigger than the maximum value the bitfield can store.
     */
    template<auto Id>
    constexpr inline Self& clear(RegisterUnderlyingType mask)
    {
        constexpr auto strongest_mask{get_maximum_value<Id>()};
        if (mask > strongest_mask)
            throw mask_not_matching_error{};

        constexpr auto shift{find_shift<Id>()};
        underlying_register &= ~(mask << shift);
        return *this;
    }

    //! Returns the underlying value.
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
