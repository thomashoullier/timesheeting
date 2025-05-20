#include "db_lib_test.h"
#include <catch2/catch_test_macros.hpp>
#include "db_lib/db_sqlite_handle.h"
#include "db_lib/db_types.h"
#include "test_utils/test_utils.h"
#include <filesystem>
#include <fstream>
#include <string>

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

TEST_CASE("DB Statement", "[db_lib]") {
  auto dir = test_utils::TempDir("timesheeting_db_lib_test");
  const std::filesystem::path db_file{dir.dirpath / "test_db.db"};
  auto db_handle = db_lib::DB_SQLite_Handle(db_file);
  auto create_table = db_handle.prepare_statement(
      "CREATE TABLE contacts (contact_id INTEGER PRIMARY KEY, "
      "name TEXT NOT NULL, "
      "active BOOLEAN CHECK (active IN (0,1)), "
      "address TEXT, department INTEGER, available BOOLEAN);");
  SECTION("LT-DBL-140 Executing statement") {
    CHECK(create_table.execute());
  }
  SECTION("LT-DBL-150 Executing invalid statement") {
    create_table.execute();
    CHECK_THROWS(create_table.execute());
  }
  SECTION("LT-DBL-160 Repeat statement execution") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES('John');");
    CHECK(insert_contact.execute());
    CHECK(insert_contact.execute());
  }
  SECTION("LT-DBL-170 Statement execution constraint violation") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES(NULL);");
    CHECK_FALSE(insert_contact.execute());
  }
  SECTION("LT-DBL-180 Statement stepping") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES('John');");
    insert_contact.execute();
    insert_contact.execute();
    auto select_contact =
        db_handle.prepare_statement("SELECT name FROM contacts;");
    CHECK(select_contact.step());
  }
  SECTION("LT-DBL-190 Statement stepping second") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES('John');");
    insert_contact.execute();
    insert_contact.execute();
    auto select_contact =
        db_handle.prepare_statement("SELECT name FROM contacts;");
    select_contact.step();
    CHECK(select_contact.step());
  }
  SECTION("LT-DBL-200 Statement stepping no more rows") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES('John');");
    insert_contact.execute();
    insert_contact.execute();
    auto select_contact =
        db_handle.prepare_statement("SELECT name FROM contacts;");
    select_contact.step();
    select_contact.step();
    CHECK_FALSE(select_contact.step());
  }
  SECTION("LT-DBL-210 Statement stepping reset") {
    create_table.execute();
    auto insert_contact = db_handle.prepare_statement(
        "INSERT INTO contacts (name) VALUES('John');");
    insert_contact.execute();
    insert_contact.execute();
    auto select_contact =
        db_handle.prepare_statement("SELECT name FROM contacts;");
    select_contact.step();
    select_contact.step();
    select_contact.step();
    CHECK(select_contact.step());
  }
  SECTION("LT-DBL-220 Statement stepping invalid") {
    create_table.execute();
    auto insert_invalid_contact = db_handle.prepare_statement
      ("INSERT INTO contacts (contact_id, name) VALUES('John', 'John');");
    CHECK_THROWS(insert_invalid_contact.step());
  }
  SECTION("LT-DBL-230 Statement binding integer") {
    create_table.execute();
    auto select_contact_id = db_handle.prepare_statement
      ("SELECT name FROM contacts WHERE contact_id = ?;");
    select_contact_id.bind_all(1);
    SUCCEED("Integer bound to statement without error.");
  }
  SECTION("LT-DBL-240 Statement binding string") {
    create_table.execute();
    auto select_contact_name = db_handle.prepare_statement
      ("SELECT contact_id FROM contacts WHERE name = ? LIMIT 1;");
    const std::string name {"John"};
    select_contact_name.bind_all(name);
    SUCCEED("String bound to statement without error.");
  }
  SECTION("LT-DBL-250 Binding too many parameters") {
    create_table.execute();
    auto select_contact_name = db_handle.prepare_statement(
        "SELECT contact_id FROM contacts WHERE name = ? LIMIT 1;");
    const std::string name_john {"John"};
    const std::string name_melissa {"Melissa"};
    CHECK_THROWS(select_contact_name.bind_all(name_john, name_melissa));
  }
}

TEST_CASE("DB statement column return", "[db_lib]") {
  auto dir = test_utils::TempDir("timesheeting_db_lib_test");
  const std::filesystem::path db_file{dir.dirpath / "test_db.db"};
  auto db_handle = db_lib::DB_SQLite_Handle(db_file);
  auto create_table = db_handle.prepare_statement(
      "CREATE TABLE contacts (contact_id INTEGER PRIMARY KEY, "
      "name TEXT NOT NULL, "
      "active BOOLEAN CHECK (active IN (0,1)), "
      "address TEXT, department INTEGER, available BOOLEAN);");
  create_table.execute();
  auto insert_john = db_handle.prepare_statement(
      "INSERT INTO contacts (name, active) VALUES('John', TRUE);");
  auto insert_melissa = db_handle.prepare_statement(
      "INSERT INTO contacts (name, active) VALUES('Melissa', FALSE);");
  auto insert_luke = db_handle.prepare_statement
    ("INSERT INTO contacts (name, active) VALUES('Luke', TRUE);");
  insert_john.execute();
  insert_melissa.execute();
  insert_luke.execute();
  SECTION("LT-DBL-260 Statement return boolean") {
    auto select_active = db_handle.prepare_statement(
        "SELECT active FROM contacts WHERE name = ? LIMIT 1;");
    const std::string name {"John"};
    select_active.bind_all(name);
    CHECK(select_active.step());
    auto [john_active] = select_active.get_all<bool>();
    CHECK(john_active);
  }
  SECTION("LT-DBL-270 Statement return integer") {
    auto select_id = db_handle.prepare_statement(
        "SELECT contact_id FROM contacts WHERE name = ?;");
    const std::string name {"Melissa"};
    select_id.bind_all(name);
    select_id.step();
    auto [melissa_id] = select_id.get_all<db_lib::RowId>();
    CHECK(melissa_id == 2);
  }
  SECTION("LT-DBL-280 Statement return string") {
    auto select_name = db_handle.prepare_statement(
        "SELECT name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(1);
    select_name.step();
    auto [name] = select_name.get_all<std::string>();
    CHECK(name == "John");
  }
  SECTION("LT-DBL-290 Statement return shortcut") {
    auto select_name = db_handle.prepare_statement
      ("SELECT contact_id, name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(2);
    auto [id, name] = select_name.single_get_all<db_lib::RowId, std::string>();
    CHECK(id == 2);
    CHECK(name == "Melissa");
  }
  SECTION("LT-DBL-300 Statement return shortcut reset") {
    auto select_name = db_handle.prepare_statement(
        "SELECT contact_id, name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(2);
    auto [id, name] = select_name.single_get_all<db_lib::RowId, std::string>();
    CHECK(id == 2);
    CHECK(name == "Melissa");
    auto [id2, name2] =
        select_name.single_get_all<db_lib::RowId, std::string>();
    CHECK(id2 == 2);
    CHECK(name2 == "Melissa");
  }
  SECTION("LT-DBL-310 Statement column return wrong number of strings") {
    auto select_name = db_handle.prepare_statement
      ("SELECT name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(1);
    select_name.step();
    CHECK_THROWS(select_name.get_all<std::string, std::string>());
  }
  SECTION("LT-DBL-320 Statement column return wrong number of booleans") {
    auto select_active = db_handle.prepare_statement
      ("SELECT active FROM contacts WHERE contact_id = ?;");
    select_active.bind_all(1);
    select_active.step();
    CHECK_THROWS(select_active.get_all<bool, bool>());
  }
  SECTION("LT-DBL-330 Statement column return wrong number of integers") {
    auto select_id = db_handle.prepare_statement
      ("SELECT contact_id FROM contacts WHERE name = ?;");
    const std::string name {"John"};
    select_id.bind_all(name);
    select_id.step();
    CHECK_THROWS(select_id.get_all<db_lib::RowId, db_lib::RowId>());
  }
  SECTION("LT-DBL-340 Statement column return wrong type integer") {
    auto select_name = db_handle.prepare_statement
      ("SELECT name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(1);
    select_name.step();
    CHECK_THROWS(select_name.get_all<db_lib::RowId>());
  }
  SECTION("LT-DBL-350 Statement column return wrong type boolean") {
    auto select_name = db_handle.prepare_statement
      ("SELECT name FROM contacts WHERE contact_id = ?;");
    select_name.bind_all(1);
    select_name.step();
    CHECK_THROWS(select_name.get_all<bool>());
  }
  SECTION("LT-DBL-360 Statement column return wrong type string") {
    auto select_id = db_handle.prepare_statement
      ("SELECT contact_id FROM contacts WHERE name = ?;");
    const std::string name {"John"};
    select_id.bind_all(name);
    select_id.step();
    CHECK_THROWS(select_id.get_all<std::string>());
  }
  SECTION("LT-DBL-370 Statement column return no rows") {
    auto select_id = db_handle.prepare_statement(
        "SELECT contact_id FROM contacts WHERE name = ?;");
    const std::string name{"Arnold"};
    select_id.bind_all(name);
    CHECK_FALSE(select_id.step());
    CHECK_THROWS(select_id.get_all<bool>());
    CHECK_THROWS(select_id.get_all<db_lib::RowId>());
    CHECK_THROWS(select_id.get_all<std::string>());
  }
  SECTION("LT-DBL-380 Statement column return null boolean") {
    auto select_available = db_handle.prepare_statement(
        "SELECT available FROM contacts WHERE contact_id = 1;");
    select_available.step();
    auto [available_result] = select_available.get_all<bool>();
    CHECK_FALSE(available_result);
  }
  SECTION("LT-DBL-390 Statement column return null integer") {
    auto select_department = db_handle.prepare_statement(
        "SELECT department FROM contacts WHERE contact_id = 1;");
    select_department.step();
    auto [department_result] = select_department.get_all<db_lib::DBInt>();
    CHECK(department_result == 0);
  }
  SECTION("LT-DBL-400 Statement column return null string") {
    auto select_address = db_handle.prepare_statement(
        "SELECT address FROM contacts WHERE contact_id = 1;");
    select_address.step();
    auto [address_result] = select_address.get_all<std::string>();
    CHECK(address_result.empty());
  }
  SECTION("LT-DBL-410 Store and query negative integers") {
    int reference_integer {-20};
    auto insert_paul = db_handle.prepare_statement(
        "INSERT INTO contacts (name, active, department) "
        "VALUES('Paul', TRUE, ?);");
    insert_paul.bind_all(reference_integer);
    SUCCEED("Negative integer bound without error.");
    insert_paul.execute();
    SUCCEED("Negative integer stored without error.");
    auto select_department = db_handle.prepare_statement(
        "SELECT department FROM contacts WHERE name = 'Paul';");
    select_department.step();
    auto [department_result] = select_department.get_all<db_lib::DBInt>();
    CHECK(department_result == reference_integer);
  }
  SECTION("LT-DBL-420 Store and query integers larger than 32 bits") {
    int64_t reference_integer{8589934592};
    auto insert_paul = db_handle.prepare_statement(
        "INSERT INTO contacts (name, active, department) "
        "VALUES('Paul', TRUE, ?);");
    insert_paul.bind_all(reference_integer);
    SUCCEED("Negative integer bound without error.");
    insert_paul.execute();
    SUCCEED("Negative integer stored without error.");
    auto select_department = db_handle.prepare_statement(
        "SELECT department FROM contacts WHERE name = 'Paul';");
    select_department.step();
    auto [department_result] = select_department.get_all<db_lib::DBInt>();
    CHECK(department_result == reference_integer);
  }
}
