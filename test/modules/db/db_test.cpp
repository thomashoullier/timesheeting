#include "db_test.h"
#include "test_utils/test_utils.h"
#include "db/db_sqlite.h"
#include "time_lib/date.h"
#include "version/version.h"

static auto db_test_folder = test_utils::TempDir("timesheeting_db_test");
static auto db_test_filepath = db_test_folder.dirpath / "db_test.db";

TEST_CASE("DB module") {
  db::DB_SQLite::get(db_test_filepath);
  SECTION("MT-DBI-010 DB singleton grab") {
    db::db();
    SUCCEED("DB singleton grabbed without error.");
  }
  SECTION("MT-DBI-020 DB get user version") {
    auto db_version = db::db().get_user_version();
    CHECK(db_version == version::TIMESHEETING_DB_VERSION);
  }
  SECTION("MT-DBI-030 Projects addition, querying and deletion") {
    const std::string project_name {"project_MT-DBI-030"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(projects.size() == 1);
    auto project = projects.front();
    CHECK(project.name == project_name);
    auto project_id = project.id;
    CHECK(db::db().delete_project(project_id));
    CHECK(db::db().query_projects().empty());
  }
  SECTION("MT-DBI-040 Locations addition, querying and deletion") {
    const std::string location_name{"location_MT-DBI-040"};
    CHECK(db::db().add_location(location_name));
    auto locations = db::db().query_locations();
    CHECK(locations.size() == 1);
    auto location = locations.front();
    CHECK(location.name == location_name);
    auto location_id = location.id;
    CHECK(db::db().delete_location(location_id));
    CHECK(db::db().query_locations().empty());
  }
  SECTION("MT-DBI-050 Tasks addition, querying and deletion") {
    const std::string parent_project_name {"project_MT-DBI-050"};
    CHECK(db::db().add_project(parent_project_name));
    auto projects = db::db().query_projects();
    auto parent_project = projects.front();
    auto project_id = parent_project.id;
    const std::string task_name{"task_MT-DBI-050"};
    CHECK(db::db().add_task(project_id, task_name));
    auto tasks = db::db().query_tasks(project_id);
    CHECK(tasks.size() == 1);
    auto task = tasks.front();
    CHECK(task.name == task_name);
    CHECK(db::db().delete_task(task.id));
    CHECK(db::db().query_tasks(project_id).empty());
    CHECK(db::db().delete_project(project_id));
  }
  SECTION("MT-DBI-060 Unique project names") {
    const std::string project_name {"project_MT-DBI-060"};
    CHECK(db::db().add_project(project_name));
    CHECK_FALSE(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(projects.size() == 1);
    auto project = projects.front();
    CHECK(project.name == project_name);
    auto project_id = project.id;
    CHECK(db::db().delete_project(project_id));
  }
  SECTION("MT-DBI-070 Unique location names") {
    const std::string location_name{"location_MT-DBI-070"};
    CHECK(db::db().add_location(location_name));
    CHECK_FALSE(db::db().add_location(location_name));
    auto locations = db::db().query_locations();
    CHECK(locations.size() == 1);
    auto location = locations.front();
    CHECK(location.name == location_name);
    CHECK(db::db().delete_location(location.id));
  }
  SECTION("MT-DBI-080 Unique task names") {
    const std::string project1_name{"project1_MT-DBI-080"};
    const std::string project2_name{"project2_MT-DBI-080"};
    CHECK(db::db().add_project(project1_name));
    CHECK(db::db().add_project(project2_name));
    auto projects = db::db().query_projects();
    const std::string task_name{"task_MT-DBI-080"};
    CHECK(db::db().add_task(projects.front().id, task_name));
    CHECK_FALSE(db::db().add_task(projects.front().id, task_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.size() == 1);
    CHECK(db::db().add_task(projects.at(1).id, task_name));
    CHECK(db::db().delete_task(tasks.front().id));
    auto other_tasks = db::db().query_tasks(projects.at(1).id);
    CHECK(db::db().delete_task(other_tasks.front().id));
    CHECK(db::db().delete_project(projects.front().id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-090 Project's tasks deletion") {
    const std::string project_name{"project_MT-DBI-090"};
    const std::string task_name{"task_MT-DBI-090"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_name));
    CHECK(db::db().delete_project(projects.front().id));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.empty());
    auto projects_now = db::db().query_projects();
    CHECK(projects_now.empty());
  }
  SECTION("MT-DBI-100 Projects active default") {
    const std::string project_name {"project_MT-DBI-100"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    auto project = projects.front();
    CHECK(project.active);
    CHECK(db::db().delete_project(project.id));
  }
  SECTION("MT-DBI-110 Locations active default") {
    const std::string location_name{"location_MT-DBI-110"};
    CHECK(db::db().add_location(location_name));
    auto locations = db::db().query_locations();
    auto location = locations.front();
    CHECK(location.active);
    CHECK(db::db().delete_location(location.id));
  }
  SECTION("MT-DBI-120 Tasks active default") {
    const std::string project_name{"project_MT-DBI-120"};
    const std::string task_name{"task_MT-DBI-120"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.front().active);
    CHECK(db::db().delete_task(tasks.front().id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-130 Query projects ordering") {
    const std::string projectA_name{"projectA_MT-DBI-130"};
    const std::string projectB_name{"projectB_MT-DBI-130"};
    CHECK(db::db().add_project(projectB_name));
    CHECK(db::db().add_project(projectA_name));
    auto projects = db::db().query_projects();
    CHECK(projects.at(0).name == projectA_name);
    CHECK(projects.at(1).name == projectB_name);
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-140 Query locations ordering") {
    const std::string locationA_name{"locationA_MT-DBI-140"};
    const std::string locationB_name{"locationB_MT-DBI-140"};
    CHECK(db::db().add_location(locationB_name));
    CHECK(db::db().add_location(locationA_name));
    auto locations = db::db().query_locations();
    CHECK(locations.at(0).name == locationA_name);
    CHECK(locations.at(1).name == locationB_name);
    CHECK(db::db().delete_location(locations.at(0).id));
    CHECK(db::db().delete_location(locations.at(1).id));
  }
  SECTION("MT-DBI-150 Query tasks ordering") {
    const std::string project_name{"project_MT-DBI-150"};
    const std::string taskA_name{"taskA_MT-DBI-150"};
    const std::string taskB_name{"taskB_MT-DBI-150"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, taskB_name));
    CHECK(db::db().add_task(projects.front().id, taskA_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.at(0).name == taskA_name);
    CHECK(tasks.at(1).name == taskB_name);
    CHECK(db::db().delete_task(tasks.at(0).id));
    CHECK(db::db().delete_task(tasks.at(1).id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-160 Toggle project active") {
    const std::string project_name{"project_MT-DBI-160"};
    CHECK(db::db().add_project(project_name));
    auto projects_first = db::db().query_projects();
    CHECK(projects_first.front().active);
    CHECK(db::db().toggle_project_active(projects_first.front().id));
    auto projects_second = db::db().query_projects();
    CHECK_FALSE(projects_second.front().active);
    CHECK(db::db().toggle_project_active(projects_second.front().id));
    auto projects_third = db::db().query_projects();
    CHECK(projects_third.front().active);
    CHECK(db::db().delete_project(projects_third.front().id));
  }
  SECTION("MT-DBI-170 Toggle location active") {
    const std::string location_name{"location_MT-DBI-170"};
    CHECK(db::db().add_location(location_name));
    auto locations_first = db::db().query_locations();
    CHECK(locations_first.front().active);
    CHECK(db::db().toggle_location_active(locations_first.front().id));
    auto locations_second = db::db().query_locations();
    CHECK_FALSE(locations_second.front().active);
    CHECK(db::db().toggle_location_active(locations_second.front().id));
    auto locations_third = db::db().query_locations();
    CHECK(locations_third.front().active);
    CHECK(db::db().delete_location(locations_third.front().id));
  }
  SECTION("MT-DBI-180 Toggle task active") {
    const std::string project_name{"project_MT-DBI-180"};
    const std::string task_name{"task_MT-DBI-180"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    auto project_id = projects.front().id;
    CHECK(db::db().add_task(projects.front().id, task_name));
    auto tasks_first = db::db().query_tasks(project_id);
    CHECK(tasks_first.front().active);
    CHECK(db::db().toggle_task_active(tasks_first.front().id));
    auto tasks_second = db::db().query_tasks(project_id);
    CHECK_FALSE(tasks_second.front().active);
    CHECK(db::db().toggle_task_active(tasks_second.front().id));
    auto tasks_third = db::db().query_tasks(project_id);
    CHECK(tasks_third.front().active);
    CHECK(db::db().delete_task(tasks_third.front().id));
    CHECK(db::db().delete_project(project_id));
  }
  SECTION("MT-DBI-190 Query active projects") {
    const std::string project_active_name{"project_active_MT-DBI-190"};
    const std::string project_inactive_name{"project_inactive_MT-DBI-190"};
    CHECK(db::db().add_project(project_active_name));
    CHECK(db::db().add_project(project_inactive_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().toggle_project_active(projects.at(1).id));
    auto active_projects = db::db().query_projects_active();
    CHECK(active_projects.size() == 1);
    CHECK(active_projects.front().name == project_active_name);
    auto projects_final = db::db().query_projects();
    CHECK(db::db().delete_project(projects_final.at(0).id));
    CHECK(db::db().delete_project(projects_final.at(1).id));
  }
  SECTION("MT-DBI-200 Query active locations") {
    const std::string location_active_name{"location_active_MT-DBI-200"};
    const std::string location_inactive_name{"location_inactive_MT-DBI-200"};
    CHECK(db::db().add_location(location_active_name));
    CHECK(db::db().add_location(location_inactive_name));
    auto locations = db::db().query_locations();
    CHECK(db::db().toggle_location_active(locations.at(1).id));
    auto active_locations = db::db().query_locations_active();
    CHECK(active_locations.size() == 1);
    CHECK(active_locations.front().name == location_active_name);
    auto locations_final = db::db().query_locations();
    CHECK(db::db().delete_location(locations_final.at(0).id));
    CHECK(db::db().delete_location(locations_final.at(1).id));
  }
  SECTION("MT-DBI-210 Query active tasks") {
    const std::string project_name{"project_MT-DBI-210"};
    const std::string task_active_name{"task_active_MT-DBI-210"};
    const std::string task_inactive_name{"task_inactive_MT-DBI-210"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_active_name));
    CHECK(db::db().add_task(projects.front().id, task_inactive_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(db::db().toggle_task_active(tasks.at(1).id));
    auto active_tasks = db::db().query_tasks_active(projects.front().id);
    CHECK(active_tasks.size() == 1);
    CHECK(active_tasks.front().name == task_active_name);
    CHECK(db::db().delete_task(tasks.at(0).id));
    CHECK(db::db().delete_task(tasks.at(1).id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-220 Query active projects ordering") {
    const std::string projectA_name{"projectA_MT-DBI-220"};
    const std::string projectB_name{"projectB_MT-DBI-220"};
    CHECK(db::db().add_project(projectB_name));
    CHECK(db::db().add_project(projectA_name));
    auto projects = db::db().query_projects_active();
    CHECK(projects.at(0).name == projectA_name);
    CHECK(projects.at(1).name == projectB_name);
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-230 Query active locations ordering") {
    const std::string locationA_name{"locationA_MT-DBI-230"};
    const std::string locationB_name{"locationB_MT-DBI-230"};
    CHECK(db::db().add_location(locationB_name));
    CHECK(db::db().add_location(locationA_name));
    auto locations = db::db().query_locations_active();
    CHECK(locations.at(0).name == locationA_name);
    CHECK(locations.at(1).name == locationB_name);
    CHECK(db::db().delete_location(locations.at(0).id));
    CHECK(db::db().delete_location(locations.at(1).id));
  }
  SECTION("MT-DBI-240 Query active tasks ordering") {
    const std::string project_name{"project_MT-DBI-240"};
    const std::string taskA_name{"taskA_MT-DBI-240"};
    const std::string taskB_name{"taskB_MT-DBI-240"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, taskB_name));
    CHECK(db::db().add_task(projects.front().id, taskA_name));
    auto tasks = db::db().query_tasks_active(projects.front().id);
    CHECK(tasks.at(0).name == taskA_name);
    CHECK(tasks.at(1).name == taskB_name);
    CHECK(db::db().delete_task(tasks.at(0).id));
    CHECK(db::db().delete_task(tasks.at(1).id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-250 Project name edit") {
    const std::string project_name{"project_MT-DBI-250"};
    const std::string new_project_name{"new_MT-DBI-250"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().edit_project_name(projects.front().id, new_project_name));
    projects = db::db().query_projects();
    CHECK(projects.size() == 1);
    CHECK(projects.front().name == new_project_name);
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-260 Project name edit unique") {
    const std::string projectA_name{"projectA_MT-DBI-260"};
    const std::string projectB_name{"projectB_MT-DBI-260"};
    CHECK(db::db().add_project(projectA_name));
    CHECK(db::db().add_project(projectB_name));
    auto projects = db::db().query_projects();
    CHECK_FALSE(db::db().edit_project_name(projects.front().id, projectB_name));
    projects = db::db().query_projects();
    CHECK(projects.size() == 2);
    CHECK(projects.at(0).name == projectA_name);
    CHECK(projects.at(1).name == projectB_name);
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-270 Location name edit") {
    const std::string location_name{"location_MT-DBI-270"};
    const std::string new_location_name{"new_MT-DBI-270"};
    CHECK(db::db().add_location(location_name));
    auto locations = db::db().query_locations();
    CHECK(db::db().edit_location_name(locations.front().id, new_location_name));
    locations = db::db().query_locations();
    CHECK(locations.size() == 1);
    CHECK(locations.front().name == new_location_name);
    CHECK(db::db().delete_location(locations.front().id));
  }
  SECTION("MT-DBI-280 Location name edit unique") {
    const std::string locationA_name{"locationA_MT-DBI-280"};
    const std::string locationB_name{"locationB_MT-DBI-280"};
    CHECK(db::db().add_location(locationA_name));
    CHECK(db::db().add_location(locationB_name));
    auto locations = db::db().query_locations();
    CHECK_FALSE(db::db().edit_location_name(locations.front().id,
                                            locationB_name));
    locations = db::db().query_locations();
    CHECK(locations.size() == 2);
    CHECK(locations.at(0).name == locationA_name);
    CHECK(locations.at(1).name == locationB_name);
    CHECK(db::db().delete_location(locations.at(0).id));
    CHECK(db::db().delete_location(locations.at(1).id));
  }
  SECTION("MT-DBI-290 Task name edit") {
    const std::string project_name{"project_MT-DBI-290"};
    const std::string task_name{"task_MT-DBI-290"};
    const std::string new_task_name{"new_MT-DBI-290"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(db::db().edit_task_name(tasks.front().id, new_task_name));
    tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.size() == 1);
    CHECK(tasks.front().name == new_task_name);
    CHECK(db::db().delete_task(tasks.front().id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-300 Task name edit unique") {
    const std::string project_name{"project_MT-DBI-300"};
    const std::string taskA_name{"taskA_MT-DBI-300"};
    const std::string taskB_name{"taskB_MT-DBI-300"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, taskA_name));
    CHECK(db::db().add_task(projects.front().id, taskB_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK_FALSE(db::db().edit_task_name(tasks.front().id, taskB_name));
    tasks = db::db().query_tasks(projects.front().id);
    CHECK(tasks.size() == 2);
    CHECK(tasks.at(0).name == taskA_name);
    CHECK(tasks.at(1).name == taskB_name);
    CHECK(db::db().delete_task(tasks.at(0).id));
    CHECK(db::db().delete_task(tasks.at(1).id));
    CHECK(db::db().delete_project(projects.front().id));
  }
  SECTION("MT-DBI-310 Task project edit") {
    const std::string projectA_name{"projectA_MT-DBI-310"};
    const std::string projectB_name{"projectB_MT-DBI-310"};
    const std::string task_name{"task_MT-DBI-310"};
    CHECK(db::db().add_project(projectA_name));
    CHECK(db::db().add_project(projectB_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_name));
    auto tasks = db::db().query_tasks(projects.front().id);
    CHECK(db::db().edit_task_project(tasks.front().id, projectB_name));
    auto tasks_A = db::db().query_tasks(projects.at(0).id);
    auto tasks_B = db::db().query_tasks(projects.at(1).id);
    CHECK(tasks_A.empty());
    CHECK(tasks_B.size() == 1);
    CHECK(tasks_B.front().name == task_name);
    CHECK(db::db().delete_task(tasks.front().id));
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-320 Task project edit unique") {
    const std::string projectA_name{"projectA_MT-DBI-320"};
    const std::string projectB_name{"projectB_MT-DBI-320"};
    const std::string task_name{"task_MT-DBI-320"};
    CHECK(db::db().add_project(projectA_name));
    CHECK(db::db().add_project(projectB_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.at(0).id, task_name));
    CHECK(db::db().add_task(projects.at(1).id, task_name));
    auto tasks_A = db::db().query_tasks(projects.front().id);
    CHECK_FALSE(db::db().edit_task_project(tasks_A.front().id, projectB_name));
    tasks_A = db::db().query_tasks(projects.at(0).id);
    auto tasks_B = db::db().query_tasks(projects.at(1).id);
    CHECK(tasks_A.size() == 1);
    CHECK(tasks_B.size() == 1);
    CHECK(db::db().delete_task(tasks_A.front().id));
    CHECK(db::db().delete_task(tasks_B.front().id));
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-330 Task project edit nonexistent") {
    const std::string projectA_name{"projectA_MT-DBI-320"};
    const std::string projectB_name{"projectB_MT-DBI-320"};
    const std::string nonexistent_name{"nonexistent_project"};
    const std::string task_name{"task_MT-DBI-320"};
    CHECK(db::db().add_project(projectA_name));
    CHECK(db::db().add_project(projectB_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.at(0).id, task_name));
    auto tasks_A = db::db().query_tasks(projects.front().id);
    CHECK_FALSE(
        db::db().edit_task_project(tasks_A.front().id, nonexistent_name));
    tasks_A = db::db().query_tasks(projects.front().id);
    CHECK(tasks_A.size() == 1);
    CHECK(tasks_A.front().name == task_name);
    CHECK(db::db().delete_task(tasks_A.front().id));
    CHECK(db::db().delete_project(projects.at(0).id));
    CHECK(db::db().delete_project(projects.at(1).id));
  }
  SECTION("MT-DBI-340 Entry staging empty at first") {
    auto entry_staging = db::db().query_entrystaging();
    SUCCEED("Entry staging queried without error.");
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
    CHECK_FALSE(entry_staging.location_name.has_value());
    CHECK_FALSE(entry_staging.start.has_value());
    CHECK_FALSE(entry_staging.stop.has_value());
  }
  SECTION("MT-DBI-350 Entries empty at first") {
    time_lib::Date start{1745817889};
    time_lib::Date stop{1746249889};
    time_lib::DateRange date_range {start, stop};
    auto entries = db::db().query_entries(date_range);
    SUCCEED("Entries queried without error.");
    CHECK(entries.empty());
  }
  SECTION("MT-DBI-360 Entry staging project name") {
    const std::string project_name {"project_MT-DBI-360"};
    const std::string task_name {"task_MT-DBI-360"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    CHECK(db::db().add_task(projects.front().id, task_name));
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    SUCCEED("Entry staging project name set without error.");
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.project_name.has_value());
    CHECK(entry_staging.project_name.value() == project_name);
  }
  SECTION("MT-DBI-370 Entry staging task auto-fill") {
    const std::string ref_task_name {"task_MT-DBI-360"};
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.task_name.has_value());
    CHECK(entry_staging.task_name.value() == ref_task_name);
  }
  SECTION("MT-DBI-380 Entry staging non-existent project name") {
    const std::string project_name{"nonexistent_project"};
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
  }
  SECTION("MT-DBI-390 Entry staging active projects") {
    const std::string project_name{"project_MT-DBI-390"};
    const std::string task_name{"task_MT-DBI-390"};
    CHECK(db::db().add_project(project_name));
    auto projects = db::db().query_projects();
    for (const auto &project : projects) {
      if (project.name == project_name) {
        CHECK(db::db().add_task(project.id, task_name));
        CHECK(db::db().toggle_project_active(project.id));
      }
    }
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
  }
  SECTION("MT-DBI-400 Entry staging task name without project") {
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK(db::db().edit_entrystaging_task_name("task_MT-DBI-360"));
    entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
  }
  SECTION("MT-DBI-410 Entry staging task name") {
    const std::string project_name {"project_MT-DBI-360"};
    const std::string task_name {"task_MT-DBI-410"};
    auto projects = db::db().query_projects();
    for (const auto &project : projects) {
      if (project.name == project_name) {
        CHECK(db::db().add_task(project.id, task_name));
      }
    }
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    CHECK(db::db().edit_entrystaging_task_name(task_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.project_name.has_value());
    CHECK(entry_staging.task_name.has_value());
    CHECK(entry_staging.project_name.value() == project_name);
    CHECK(entry_staging.task_name.value() == task_name);
  }
  SECTION("MT-DBI-420 Entry staging task name active") {
    const std::string project_name{"project_MT-DBI-360"};
    const std::string task_name {"task_MT-DBI-420"};
    auto projects = db::db().query_projects();
    for (const auto &project : projects) {
      if (project.name == project_name) {
        CHECK(db::db().add_task(project.id, task_name));
        auto tasks = db::db().query_tasks(project.id);
        for (const auto &task : tasks) {
          if (task.name == task_name) {
            CHECK(db::db().toggle_task_active(task.id));
          }
        }
      }
    }
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    CHECK(db::db().edit_entrystaging_task_name(task_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
  }
  SECTION("MT-DBI-430 Entry staging task name non-existent") {
    const std::string project_name{"project_MT-DBI-360"};
    const std::string task_name{"nonexistent_task"};
    CHECK(db::db().edit_entrystaging_project_name(project_name));
    CHECK(db::db().edit_entrystaging_task_name(task_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.project_name.has_value());
    CHECK_FALSE(entry_staging.task_name.has_value());
  }
  SECTION("MT-DBI-440 Entry staging location name") {
    const std::string location_name{"location_MT-DBI-440"};
    CHECK(db::db().add_location(location_name));
    CHECK(db::db().edit_entrystaging_location_name(location_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.location_name.has_value());
    CHECK(entry_staging.location_name.value() == location_name);
  }
  SECTION("MT-DBI-450 Entry staging location name active") {
    const std::string location_name{"location_MT-DBI-450"};
    CHECK(db::db().add_location(location_name));
    auto locations = db::db().query_locations();
    for (const auto &location : locations) {
      if (location.name == location_name) {
        CHECK(db::db().toggle_location_active(location.id));
      }
    }
    CHECK(db::db().edit_entrystaging_location_name(location_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.location_name.has_value());
  }
  SECTION("MT-DBI-460 Entry staging location name non-existent") {
    const std::string location_name{"nonexistent_location"};
    CHECK(db::db().edit_entrystaging_location_name(location_name));
    auto entry_staging = db::db().query_entrystaging();
    CHECK_FALSE(entry_staging.location_name.has_value());
  }
  SECTION("MT-DBI-470 Entry staging start date") {
    const time_lib::Date start_date{1745818889};
    CHECK(db::db().edit_entrystaging_start(start_date));
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.start.has_value());
    CHECK(entry_staging.start.value().to_unix_timestamp() ==
          start_date.to_unix_timestamp());
  }
  SECTION("MT-DBI-480 Entry staging stop date") {
    const time_lib::Date stop_date{1745819889};
    CHECK(db::db().edit_entrystaging_stop(stop_date));
    auto entry_staging = db::db().query_entrystaging();
    CHECK(entry_staging.stop.has_value());
    CHECK(entry_staging.stop.value().to_unix_timestamp() ==
          stop_date.to_unix_timestamp());
  }
}
