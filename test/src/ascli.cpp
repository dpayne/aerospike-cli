#include <doctest/doctest.h>
#include <ascli/ascli.h>
#include <ascli/version.h>

#include <string>

TEST_CASE("AsCli") {
  using namespace ascli;

  AsCli ascli("Tests");

  CHECK(ascli.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(ascli.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(ascli.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(ascli.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("AsCli version") {
  static_assert(std::string_view(ASCLI_VERSION) == std::string_view("1.0"));
  CHECK(std::string(ASCLI_VERSION) == std::string("1.0"));
}
