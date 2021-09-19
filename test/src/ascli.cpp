#include <ascli/ascli.h>
#include <ascli/version.h>
#include <doctest/doctest.h>

#include <string>

TEST_CASE("AsCli") {
    using namespace ascli;
}

TEST_CASE("AsCli version") {
    static_assert(std::string_view(ASCLILIB_VERSION) == std::string_view("1.0"));
    CHECK(std::string(ASCLILIB_VERSION) == std::string("1.0"));
}
