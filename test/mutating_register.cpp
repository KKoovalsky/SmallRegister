/**
 * @file	mutating_register.cpp
 * @brief	Test of chained setting and clearing during runtime.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "catch2/catch_test_macros.hpp"
#include "small_register/small_register.hpp"

#include <catch2/catch_all.hpp>

#include "helpers.hpp"

using namespace jungles;

TEST_CASE("Bits are mutated in the run", "[small_register][one_byte_size][mutate_in_run]")
{

    static constexpr small_register<bits<reg::one, 2>, bits<reg::two, 3>, bits<reg::three, 3>> some_reg;

    SECTION("Setting and then setting works")
    {
        SECTION("For chained operation")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::three>{0b100}).set(bitfield<reg::two>{0b010})()
                    == 0b10010100);
        }

        SECTION("For procedural code")
        {
            auto r{some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::three>{0b100})};
            auto rr{r.set(bitfield<reg::two>{0b010})};
            REQUIRE(rr() == 0b10010100);
        }
    }

    SECTION("Setting and then clearing works")
    {
        SECTION("For chained operation")
        {
            REQUIRE(some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::three>{0b110})
                        .clear_individual_bits(bitfield<reg::three>{0b010})()
                    == 0b10000100);
        }

        SECTION("For procedural code")
        {
            auto r{some_reg.set(bitfield<reg::one>{0b10}, bitfield<reg::three>{0b110})};
            auto rr{r.clear_individual_bits(bitfield<reg::three>{0b010})};
            REQUIRE(rr() == 0b10000100);
        }

        SECTION("For a long chain")
        {
            SECTION("For chained operation")
            {
                auto r{some_reg.set(bitfield<reg::one>{0b11}, bitfield<reg::two>{0b010}, bitfield<reg::three>{0b101})
                           .clear_individual_bits(bitfield<reg::one>{0b01}, bitfield<reg::three>{0b001})
                           .set(bitfield<reg::two>{0b001}, bitfield<reg::three>{0b011})
                           .clear_individual_bits(bitfield<reg::two>{0b100}, bitfield<reg::three>{0b100})};

                REQUIRE(r() == 0b10011011);
            }

            SECTION("For procedural code")
            {
                auto r{some_reg.set(bitfield<reg::one>{0b11}, bitfield<reg::two>{0b010}, bitfield<reg::three>{0b101})};
                r.clear_individual_bits(bitfield<reg::one>{0b01}, bitfield<reg::three>{0b001});
                r.set(bitfield<reg::two>{0b001}, bitfield<reg::three>{0b011});
                r.clear_individual_bits(bitfield<reg::two>{0b100}, bitfield<reg::three>{0b100});

                REQUIRE(r() == 0b10011011);
            }
        }
    }
}

