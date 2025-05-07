#ifndef EXPORTER_TEST_H
#define EXPORTER_TEST_H

#include "core/export_row.h"
#include <catch2/catch_test_macros.hpp>
#include <generator>
#include <vector>

std::generator<core::ExportRow> exporter_vec_to_generator
(const std::vector<core::ExportRow> &rows_vec);

#endif // EXPORTER_TEST_H
