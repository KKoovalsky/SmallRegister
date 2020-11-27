/**
 * @file	small_details.hpp
 * @brief	Contains internal helpers.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SMALL_DETAILS_HPP
#define SMALL_DETAILS_HPP

#include <utility>

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

} // namespace jungles

#endif /* SMALL_DETAILS_HPP */
