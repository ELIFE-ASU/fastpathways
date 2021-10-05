#include <catch2/catch.hpp>
#include <fastpathways.h>

TEST_CASE("integer-chains", "[int64]" ) {
    std::vector<int64_t> table = {
        0, 1, 2, 2, 3, 3, 4, 3, 4, 4,
        5, 4, 5, 5, 5, 4, 5, 5, 6, 5,
        6, 6, 6, 5, 6, 6, 6, 6, 7, 6,
        7, 5, 6, 6, 7, 6, 7, 7, 7, 6,
        7, 7, 7, 7, 7, 7, 8, 6, 7, 7,
        7, 7, 8, 7, 8, 7, 8, 8, 8, 7,
        8, 8, 8, 6, 7, 7, 8, 7, 8, 8,
        9, 7, 8, 8, 8, 8, 8, 8, 9, 7,
        8, 8, 8, 8, 8, 8, 9, 8, 9, 8,
        9, 8, 9, 9, 9, 7, 8, 8, 8, 8
    };

    SECTION("first 100 integers") {
        for (std::size_t n = 1; n <= std::size(table); ++n) {
            REQUIRE(thurber(static_cast<int64_t>(n)) == table.at(n-1));
        }
    }

    table = { 1, 2, 3, 5, 7, 11, 19, 29, 47, 71, 127, 191, 379, 607, 1087, 1903, 3583, 6271 };

    SECTION("first 18 lengths") {
        for (std::size_t n = 0; n < std::size(table); ++n) {
            auto const expected = static_cast<int64_t>(n);
            REQUIRE(thurber(table.at(n)) == expected);
        }
    }

    SECTION("innovations") {
        for (std::size_t i = 1; i < std::size(table); ++i) {
            auto const expected = static_cast<int64_t>(i);
            for (auto n = table.at(i); n < table.at(i); ++n) {
                REQUIRE(thurber(n) < expected);
            }
        }
    }
}
