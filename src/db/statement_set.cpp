#include "statement_set.h"

StatementSet::StatementSet(std::shared_ptr<DB_SQLite_Handle> db)
    : select_projects(db->prepare_statement("SELECT id, name FROM projects;")),
      select_projects_active(db->prepare_statement(
          "SELECT id, name FROM projects WHERE active = TRUE;")),
      select_tasks(db->prepare_statement("SELECT id, name "
                                         "FROM tasks "
                                         "WHERE project_id = ? ;")),
      select_tasks_active(db->prepare_statement("SELECT id, name "
                                                "FROM tasks "
                                                "WHERE project_id = ? "
                                                "AND active = TRUE;")),
      select_locations(
          db->prepare_statement("SELECT id, name FROM locations;")),
      select_locations_active(
          db->prepare_statement("SELECT id, name FROM locations "
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
      select_duration(
          db->prepare_statement("SELECT SUM(stop - start) FROM entries e "
                                "WHERE e.start >= ? "
                                "AND e.start < ? ;"))

{}
