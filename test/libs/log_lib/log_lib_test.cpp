#include "log_lib_test.h"
#include "test_utils/test_utils.h"
#include "log_lib/logger.h"
#include "time_lib/date.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

TEST_CASE("Logger initialization error cases", "[log_lib]") {
  auto dir = test_utils::TempDir("timesheeting_db_lib_test");
  const std::vector<std::string> log_levels {"info"};
  SECTION("LT-LOG-010 Opening log file in non-existent folder") {
    const std::filesystem::path log_file{dir.dirpath / "non_existent" /
                                         "test_log.log"};
    CHECK_THROWS(log_lib::Logger::get(log_file, log_levels, 180));
  }
  SECTION("LT-LOG-020 Unknown log levels exception") {
    const std::filesystem::path log_file{dir.dirpath / "test_log.log"};
    const std::vector<std::string> wrong_log_levels {"wrong_level"};
    CHECK_THROWS(log_lib::Logger::get(log_file, wrong_log_levels, 180));
  }
  SECTION("LT-LOG-030 Log file invalid") {
    const std::filesystem::path log_path{dir.dirpath / "test_log.log"};
    {
      std::ofstream log_file{log_path, std::ios::out};
      if (not log_file.good())
        throw std::runtime_error("LT-LOG-030 Log file invalid: log file could "
                                 "not be prepared!");
      log_file << "NOT AN ENTRY" << std::endl;
    }
    CHECK_THROWS(log_lib::Logger::get(log_path, log_levels, 180));
  }
}

static const auto valid_log_dir =
  test_utils::TempDir{"static_timesheeting_db_lib_test"};

TEST_CASE("Logger valid initialization", "[log_lib]") {
  const std::filesystem::path log_filepath
    {valid_log_dir.dirpath / "test_log.log"};
  const std::vector<std::string> log_levels{"info", "debug"};
  const uint64_t max_age{180};
  if (not std::filesystem::exists(log_filepath)) {
    std::ofstream log_file{log_filepath, std::ios::out};
    if (not log_file.good())
      throw std::runtime_error("Logger valid initialization: log file could "
                               "not be prepared!");
    time_lib::Date now_date{};
    time_lib::Date old_date{now_date.to_unix_timestamp() - 300};
    time_lib::Date new_date{now_date.to_unix_timestamp() - 60};
    log_file << old_date.to_fullstring()
             << " [debug] : Older entry, to remove.\n";
    log_file << new_date.to_fullstring()
             << " [debug] : Newer entry, to keep." << std::endl;
  }
  log_lib::Logger::get(log_filepath, log_levels, max_age);
  SECTION("LT-LOG-040 Log valid initialization") {
    SUCCEED("Logger initialized without error.");
  }
  SECTION("LT-LOG-050 Log cleanup") {
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 1);
    const std::string line = lines.front();
    const std::string reference_suffix{" [debug] : Newer entry, to keep."};
    CHECK(line.size() > reference_suffix.size());
    const auto start_pos = line.size() - reference_suffix.size();
    CHECK(lines.front().substr(start_pos) == reference_suffix);
  }
  SECTION("LT-LOG-060 Logging") {
    const std::string message{"Basic log message."};
    log_lib::logger().log(message);
    SUCCEED("Message logged without error.");
  }
  SECTION("LT-LOG-070 Log entry format") {
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 2);
    const std::string line = lines.back();
    const std::string reference_suffix{" [info] : Basic log message."};
    const auto start_pos = line.size() - reference_suffix.size();
    CHECK(line.substr(start_pos) == reference_suffix);
    const time_lib::Date entry_date{time_lib::Date::FullString{},
                                    line.substr(0, start_pos)};
    CHECK(time_lib::Date{}.to_unix_timestamp() -
              entry_date.to_unix_timestamp() <=
          10);
  }
  SECTION("LT-LOG-080 Logging custom active level") {
    const std::string message{"Custom active log level."};
    log_lib::Logger::get().log(message, log_lib::LogLevel::debug);
    SUCCEED("Log called without error.");
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 3);
    const std::string line = lines.back();
    const std::string reference_suffix{" [debug] : Custom active log level."};
    const auto start_pos = line.size() - reference_suffix.size();
    CHECK(line.substr(start_pos) == reference_suffix);
  }
  SECTION("LT-LOG-090 Logging custom inactive level") {
    const std::string message{"Custom inactive log level."};
    log_lib::Logger::get().log(message, log_lib::LogLevel::error);
    SUCCEED("Log called without error.");
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 3);
  }
  SECTION("LT-LOG-100 Log chronometer") {
    log_lib::Logger::get().tick();
    SUCCEED("Tick called without error.");
    log_lib::Logger::get().tock();
    SUCCEED("Tock called without error.");
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 4);
  }
  SECTION("LT-LOG-110 Log chronometer message") {
    log_lib::Logger::get().tick();
    SUCCEED("Tick called without error.");
    const std::string message {"Chronometer duration: "};
    log_lib::Logger::get().tock(message);
    SUCCEED("Tock called with message without error.");
    auto lines = test_utils::file_as_strings(log_filepath);
    CHECK(lines.size() == 5);
  }
  SECTION("LT-LOG-120 Log chronometer entry format") {
    auto lines = test_utils::file_as_strings(log_filepath);
    auto line_nomessage = lines.at(lines.size() - 2);
    auto line_message = lines.back();
    // Parsable full dates
    CHECK(line_nomessage.size() > 24);
    CHECK(line_message.size() > 24);
    auto date_str_1 = line_nomessage.substr(0, 24);
    auto date_str_2 = line_nomessage.substr(0, 24);
    time_lib::Date date_1{time_lib::Date::FullString{}, date_str_1};
    time_lib::Date date_2{time_lib::Date::FullString{}, date_str_2};
    SUCCEED("Dates parsed successfully.");
    // Log level string
    const std::string reference_level {" [debug] : "};
    CHECK(line_nomessage.substr(24, reference_level.size()) ==
          reference_level);
    CHECK(line_message.substr(24, reference_level.size()) ==
          reference_level);
    // String prefix.
    const std::string reference_prefix {" ms."};
    CHECK(line_nomessage.substr(line_nomessage.size() -
                                reference_prefix.size()) == reference_prefix);
    CHECK(line_message.substr(line_message.size() -
                              reference_prefix.size()) == reference_prefix);
    // Duration values
    std::regex re("[0-9]+\\.[0-9]{3}");
    std::smatch re_match_1, re_match_2;
    const std::string sub1 = line_nomessage.substr(24);
    const std::string sub2 = line_message.substr(24);
    CHECK(std::regex_search(sub1, re_match_1, re));
    CHECK(std::regex_search(sub2, re_match_2, re));
    std::string match_1, match_2;
    for (std::size_t i = 0 ; i < re_match_1.size() ; ++i) {
      match_1 += re_match_1[i];
    }
    for (std::size_t i = 0; i < re_match_2.size(); ++i) {
      match_2 += re_match_2[i];
    }
    CHECK(std::stof(match_1) < 1000.0f);
    CHECK(std::stof(match_2) < 1000.0f);
  }
  SECTION("LT-LOG-130 Log message multiline exception") {
    const std::string message{"Multi\nline."};
    CHECK_THROWS(log_lib::Logger::get().log(message));
  }
  SECTION("LT-LOG-140 Chronometer message") {
    log_lib::Logger::get().tick();
    const std::string message{"Multi\nline."};
    CHECK_THROWS(log_lib::Logger::get().tock(message));
  }
}
