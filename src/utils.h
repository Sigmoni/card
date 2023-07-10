#pragma once

#define QUERY_WORKLOAD 1000

#include "rapidcsv.h"
#include <iostream>
#include <string>
#include <tabulate/table.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <vector>

typedef std::pair<float, float> Query;

tabulate::Table GetColumnInfo(rapidcsv::Document &doc);
void DoExperiment(std::string columnset, std::vector<std::string> const &fst,
                  std::vector<std::string> const &snd, int dv_fst, int dv_snd,
                  tabulate::Table &summary1, tabulate::Table &summary2,
                  std::ofstream &result, tabulate::MarkdownExporter &exporter);
