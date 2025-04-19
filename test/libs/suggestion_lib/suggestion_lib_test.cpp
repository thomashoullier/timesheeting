#include "suggestion_lib_test.h"
#include <catch2/catch_test_macros.hpp>
#include "suggestion/suggestion.h"

TEST_CASE("Suggestion matcher", "[suggestion_lib]") {
  SECTION("LT-SUG-010 Empty set of choice strings") {
    std::string query {"fox"};
    std::vector<std::string> choices {};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val.empty());
  }
  SECTION("LT-SUG-020 Empty query string") {
    std::string query {};
    std::vector<std::string> choices {"banana", "fox"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val.empty());
  }
  SECTION("LT-SUG-030 Prefix match with case") {
    std::string query {"Bana"};
    std::vector<std::string> choices {"Banana", "Fox"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "Banana");
  }
  SECTION("LT-SUG-040 Prefix match without case") {
    std::string query{"fo"};
    std::vector<std::string> choices {"Banana", "Fox"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "Fox");
  }
  SECTION("LT-SUG-050 Base substring case") {
    std::string query{"ana"};
    std::vector<std::string> choices {"banana"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "banana");
  }
  SECTION("LT-SUG-060 Earliest substring") {
    std::string query{"ana"};
    std::vector<std::string> choices {"bbanana", "banana"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "banana");
  }
  SECTION("LT-SUG-070 Shortest substring") {
    std::string query{"ana"};
    std::vector<std::string> choices {"banana", "bananana"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "banana");
  }
  SECTION("LT-SUG-080 First alphabetical substring") {
    std::string query{"ana"};
    std::vector<std::string> choices {"banana", "banane"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "banana");
  }
  SECTION("LT-SUG-090 Out of order") {
    std::string query{"fence over jumps"};
    std::vector<std::string> choices {"The brown fox",
                                      "jumps over the fence"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "jumps over the fence");
  }
  SECTION("LT-SUG-100 Small typo") {
    std::string query{"juppiter"};
    std::vector<std::string> choices {"Jupiter", "Saturn"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "Jupiter");
  }
  SECTION("LT-SUG-110 No match") {
    std::string query{"yyy"};
    std::vector<std::string> choices {"banana", "fox"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val.empty());
  }
  SECTION("LT-SUG-120 Non-letters") {
    std::string query{"bana"};
    std::vector<std::string> choices {"%/.$BAnafu",
                                      "Fox"};
    std::string ret_val = suggestion::best_match(query, choices);
    CHECK(ret_val == "%/.$BAnafu");
  }
}
