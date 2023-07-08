#pragma once

#define EPSILON 0.25

#include "rapidcsv.h"
#include <iostream>
#include <string>
#include <tabulate/table.hpp>
#include <vector>

tabulate::Table GetColumnInfo(rapidcsv::Document &doc);
tabulate::Table DoExperiment(std::vector<std::string> const &fst,
                             std::vector<std::string> const &snd, int bkt_fst,
                             int bkt_snd, int dv_fst, int dv_snd);