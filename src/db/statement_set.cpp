#include "statement_set.h"

StatementSet::StatementSet(std::shared_ptr<db_lib::DB_SQLite_Handle> db)
    : select_projects(db->prepare_statement(
          "SELECT id, name FROM projects ORDER BY name ASC;")),
      select_projects_active(db->prepare_statement(
          "SELECT id, name FROM projects WHERE active = TRUE "
          "ORDER BY name ASC;")),
      select_tasks(db->prepare_statement(
          "SELECT id, name "
          "FROM tasks "
          "WHERE project_id = ? "
          "ORDER BY name ASC;")),
      select_tasks_active(db->prepare_statement(
          "SELECT id, name "
          "FROM tasks "
          "WHERE project_id = ? "
          "AND active = TRUE "
          "ORDER BY name ASC;")),
      select_locations(db->prepare_statement(
          "SELECT id, name FROM locations ORDER BY name ASC;")),
      select_locations_active(db->prepare_statement(
          "SELECT id, name FROM locations "
          "WHERE active = TRUE ORDER BY name ASC;")),
      select_entries(db->prepare_statement(
          "SELECT e.id, p.name, t.name, e.start, e.stop, l.name "
          "FROM entries e "
          "INNER JOIN locations l ON e.location_id = l.id "
          "INNER JOIN tasks t ON e.task_id = t.id "
          "INNER JOIN projects p ON t.project_id = p.id "
          "WHERE e.start >= ? "
          "AND e.start < ? "
          "ORDER BY e.start ASC;")),
      select_entry_project_id(db->prepare_statement(
          "SELECT t.project_id FROM entries e "
          "INNER JOIN tasks t ON e.task_id = t.id "
          "WHERE e.id = ?;")),
      select_export_entries(db->prepare_statement(
          "SELECT e.id, p.id, p.name, t.id, t.name, l.id, l.name, "
          "e.start, e.stop "
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
      select_entrystaging_project_id(db->prepare_statement(
          "SELECT t.project_id "
          "FROM entrystaging es "
          "INNER JOIN tasks t ON es.task_id = t.id;")),
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
      update_task_project(db->prepare_statement(
         "UPDATE tasks "
         "SET project_id = ("
         "SELECT projects.id FROM projects "
         "WHERE projects.name = ?) "
         "WHERE id = ?;")),
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
         "WHERE id = ?")),
      update_entry_project(db->prepare_statement(
         "UPDATE entries "
         "SET task_id = ("
         "SELECT tasks.id FROM tasks "
         "INNER JOIN projects ON tasks.project_id = projects.id "
         "WHERE projects.name = ?) "
         "WHERE entries.id = ? LIMIT 1;")),
      update_entry_task(db->prepare_statement(
         "UPDATE entries "
         "SET task_id = ("
         "SELECT id FROM tasks "
         "WHERE name = ?1 "
         "AND project_id = ("
         "SELECT projects.id FROM entries "
         "INNER JOIN tasks ON entries.task_id = tasks.id "
         "INNER JOIN projects ON tasks.project_id = projects.id "
         "WHERE entries.id = ?2)) "
         "WHERE entries.id = ?2;")),
      update_entry_start(db->prepare_statement(
         "UPDATE entries SET start = ? WHERE id = ?;")),
      update_entry_stop(db->prepare_statement(
         "UPDATE entries SET stop = ? WHERE id = ?;")),
      update_entry_location(db->prepare_statement(
         "UPDATE entries SET location_id = ("
         "SELECT id FROM locations "
         "WHERE name = ?) "
         "WHERE id = ?;")),
      update_entrystaging_project(db->prepare_statement(
         "UPDATE entrystaging "
         "SET task_id = ("
         "SELECT tasks.id FROM tasks "
         "INNER JOIN projects ON tasks.project_id = projects.id "
         "WHERE projects.name = ? "
         "AND projects.active = TRUE "
         "AND tasks.active = TRUE"
         ") LIMIT 1;")),
      update_entrystaging_task(db->prepare_statement(
         "UPDATE entrystaging "
         "SET task_id = ("
         "SELECT id FROM tasks "
         "WHERE name = ? "
         "AND active = TRUE "
         "AND project_id = ("
         "SELECT projects.id FROM entrystaging "
         "INNER JOIN tasks ON entrystaging.task_id = tasks.id "
         "INNER JOIN projects ON tasks.project_id = projects.id "
         "));")),
      update_entrystaging_start(db->prepare_statement(
         "UPDATE entrystaging SET start = ?;")),
      update_entrystaging_stop(db->prepare_statement(
         "UPDATE entrystaging SET stop = ?;")),
      update_entrystaging_location(db->prepare_statement(
         "UPDATE entrystaging SET location_id = ("
         "SELECT id FROM locations "
         "WHERE name = ? "
         "AND active = TRUE);")),
      remove_task(db->prepare_statement(
         "DELETE FROM tasks WHERE id = ?;")),
      remove_project(db->prepare_statement(
         "DELETE FROM projects WHERE id = ?;")),
      remove_location(db->prepare_statement(
         "DELETE FROM locations WHERE id = ?;")),
      remove_entry(db->prepare_statement(
         "DELETE FROM entries WHERE id = ?;")),
      insert_entrystaging(db->prepare_statement(
         "INSERT INTO entries (task_id, start, stop, location_id) "
         "SELECT task_id, start, stop, location_id "
         "FROM entrystaging;")),
      sum_duration_per_project(db->prepare_statement(
         "SELECT projects.name, SUM(entries.stop - entries.start) "
         "FROM projects "
         "INNER JOIN tasks ON projects.id = tasks.project_id "
         "INNER JOIN entries ON tasks.id = entries.task_id "
         "WHERE entries.start >= ? "
         "AND entries.start < ? "
         "GROUP BY projects.name;")),
      project_duration(db->prepare_statement(
         "SELECT SUM(entries.stop - entries.start) "
         "FROM projects "
         "INNER JOIN tasks ON projects.id = tasks.project_id "
         "INNER JOIN entries ON tasks.id = entries.task_id "
         "WHERE projects.id = ? "
         "AND entries.start >= ? "
         "AND entries.start < ?;")),
      task_duration(db->prepare_statement(
         "SELECT SUM(entries.stop - entries.start) "
         "FROM tasks "
         "INNER JOIN entries ON tasks.id = entries.task_id "
         "WHERE tasks.id = ? "
         "AND entries.start >= ? "
         "AND entries.start < ?;")),
      duration_per_worked_project(db->prepare_statement(
         "SELECT projects.id, projects.name, SUM(entries.stop - entries.start) "
         "FROM projects "
         "INNER JOIN tasks ON projects.id = tasks.project_id "
         "INNER JOIN entries ON tasks.id = entries.task_id "
         "WHERE entries.start >= ? "
         "AND entries.start < ? "
         "GROUP BY projects.name " // TODO: group by id rather no?
         "HAVING SUM(entries.stop - entries.start) > 0;")),
      duration_per_worked_task(db->prepare_statement(
         "SELECT tasks.id, tasks.name, SUM(entries.stop - entries.start) "
         "FROM tasks "
         "INNER JOIN entries ON tasks.id = entries.task_id "
         "WHERE tasks.project_id = ? "
         "AND entries.start >= ? "
         "AND entries.start < ? "
         "GROUP BY tasks.name " // TODO: group by id?
         "HAVING SUM(entries.stop - entries.start) > 0;"))
         {}
