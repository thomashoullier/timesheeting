#include "statement_set.h"

StatementSet::StatementSet(std::shared_ptr<DB_SQLite_Handle> db)
    : select_tasks(db->prepare_statement("SELECT id, name "
                                         "FROM tasks "
                                         "WHERE project_id = ? ;")) {}
