#include "statement_set.h"

StatementSet::StatementSet(std::shared_ptr<DB_SQLite_Handle> db)
    : select_projects(db->prepare_statement(
          "SELECT id, name FROM projects;")),
      select_projects_active(db->prepare_statement(
          "SELECT id, name FROM projects WHERE active = TRUE;")),
      select_tasks(db->prepare_statement(
          "SELECT id, name "
          "FROM tasks "
          "WHERE project_id = ? ;")),
      select_tasks_active(db->prepare_statement(
          "SELECT id, name "
          "FROM tasks "
          "WHERE project_id = ? "
          "AND active = TRUE;")),
      select_locations(db->prepare_statement(
          "SELECT id, name FROM locations;")),
      select_locations_active(db->prepare_statement(
          "SELECT id, name FROM locations "
          "WHERE active = TRUE;")),
      select_entries(db->prepare_statement(
          "SELECT e.id, p.name, t.name, e.start, e.stop, l.name "
          "FROM entries e "
          "INNER JOIN locations l ON e.location_id = l.id "
          "INNER JOIN tasks t ON e.task_id = t.id "
          "INNER JOIN projects p ON t.project_id = p.id "
          "WHERE e.start >= ? "
          "AND e.start < ? "
          "ORDER BY e.start ASC;")),
      select_duration(db->prepare_statement(
          "SELECT SUM(stop - start) FROM entries e "
          "WHERE e.start >= ? "
          "AND e.start < ? ;")),
      select_entrystaging(db->prepare_statement(
          "SELECT p.name, t.name, start, stop, l.name "
          "FROM entrystaging "
          "LEFT JOIN locations l ON entrystaging.location_id = l.id "
          "LEFT JOIN tasks t ON entrystaging.task_id = t.id "
          "LEFT JOIN projects p ON t.project_id = p.id "
          ";")),
      insert_project(db->prepare_statement(
          "INSERT INTO projects (name, active) "
          "VALUES (?, TRUE);")),
      insert_task(db->prepare_statement(
          "INSERT INTO tasks (project_id, name, active) "
          "VALUES (?, ?, TRUE);")),
      insert_location(db->prepare_statement(
         "INSERT INTO locations (name, active) "
         "VALUES (?, 1);")),
      update_project_name(db->prepare_statement(
         "UPDATE projects "
         "SET name = ? WHERE id = ?;")),
      update_task_name(db->prepare_statement(
         "UPDATE tasks SET name = ? WHERE id = ?;")),
      update_location_name(db->prepare_statement(
         "UPDATE locations SET name = ? WHERE id = ?;")),
      toggle_location_flag(db->prepare_statement(
         "UPDATE locations SET active = NOT active "
         "WHERE id = ?;")),
      toggle_task_flag(db->prepare_statement(
         "UPDATE tasks SET active = NOT active "
         "WHERE id = ?;")),
      toggle_project_flag(db->prepare_statement(
         "UPDATE projects SET active = NOT active "
         "WHERE id = ?"))
{}
