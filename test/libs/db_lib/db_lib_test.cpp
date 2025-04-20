#include "db_lib_test.h"
#include <catch2/catch_test_macros.hpp>
#include "db_lib/db_sqlite_handle.h"
#include "test_utils/test_utils.h"
#include <filesystem>
#include <fstream>

TEST_CASE("DB handle initialization", "[db_lib]") {
  auto dir = test_utils::TempDir("timesheeting_db_lib_test");
  SECTION("LT-DBL-010 Opening new DB") {
    const std::filesystem::path db_file{dir.dirpath / "test_db.db"};
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    SUCCEED("New DB file opened without error.");
  }
  SECTION("LT-DBL-080 Opening DB in non-existent folder") {
    const std::filesystem::path db_file{dir.dirpath / "non_existent" /
                                        "test_db.db"};
    CHECK_THROWS(db_lib::DB_SQLite_Handle(db_file));
  }
  SECTION("LT-DBL-090 Opening an invalid DB file") {
    const std::filesystem::path db_file{dir.dirpath / "test_db.txt"};
    {
      std::ofstream out{db_file};
      out << "Hello" << std::endl;
    }
    CHECK_THROWS(db_lib::DB_SQLite_Handle(db_file));
  }
}

TEST_CASE("DB handle version and saving", "[db_lib]") {
  auto dir = test_utils::TempDir("timesheeting_db_lib_test");
  const std::filesystem::path db_file{dir.dirpath / "test_db.db"};
  SECTION("LT-DBL-020 Set user version") {
    {
      auto db_handle = db_lib::DB_SQLite_Handle(db_file);
      db_handle.check_user_version(3);
      SUCCEED("DB version initialized without error.");
    }
    SUCCEED("LT-DBL-030 DB was closed without error.");
  }
  SECTION("LT-DBL-040 Opening existing DB") {
    {
      auto db_handle = db_lib::DB_SQLite_Handle(db_file);
      db_handle.check_user_version(3);
    }
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    SUCCEED("DB re-opened without error.");
  }
  SECTION("LT-DBL-050 User version check") {
    {
      auto db_handle = db_lib::DB_SQLite_Handle(db_file);
      db_handle.check_user_version(3);
    }
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    db_handle.check_user_version(3);
    SUCCEED("DB version checked without error.");
  }
  SECTION("LT-DBL-060 User version check exception") {
    {
      auto db_handle = db_lib::DB_SQLite_Handle(db_file);
      db_handle.check_user_version(3);
    }
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    CHECK_THROWS(db_handle.check_user_version(2));
  }
  SECTION("LT-DBL-070 User version get") {
    {
      auto db_handle = db_lib::DB_SQLite_Handle(db_file);
      db_handle.check_user_version(3);
    }
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    auto db_version = db_handle.get_user_version();
    CHECK(db_version == 3);
  }
  SECTION("LT-DBL-100 Executing a valid statement") {
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    std::string statement_string {"PRAGMA user_version = 4;"};
    db_handle.exec_statement(statement_string);
    SUCCEED("Statement executed without error.");
    auto version = db_handle.get_user_version();
    CHECK(version == 4);
  }
  SECTION("LT-DBL-110 Executing an invalid statement") {
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    std::string statement_string{"PRAGMA user_version = ;"};
    CHECK_THROWS(db_handle.exec_statement(statement_string));
  }
  SECTION("LT-DBL-120 Preparing a valid statement") {
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    std::string statement_string{"PRAGMA user_version;"};
    db_handle.prepare_statement(statement_string);
    SUCCEED("Statement prepared without error.");
  }
  SECTION("LT-DBL-130 Preparing an invalid statement") {
    auto db_handle = db_lib::DB_SQLite_Handle(db_file);
    std::string statement_string{"NOTASTATEMENT;"};
    CHECK_THROWS(db_handle.prepare_statement(statement_string));
  }
}
