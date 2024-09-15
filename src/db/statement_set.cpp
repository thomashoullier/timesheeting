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
      select_locations(db->prepare_statement("SELECT id, name FROM locations;")),
      select_locations_active
      (db->prepare_statement("SELECT id, name FROM locations "
                             "WHERE active = TRUE;")) {}
