#include "utils.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/table.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

bool isInt(std::string const &str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isFloat(std::string const &str) {
  char *p;
  strtod(str.c_str(), &p);
  return *p == 0;
}

tabulate::Table GetColumnInfo(rapidcsv::Document &doc) {
  tabulate::Table columnInfoTable;

  columnInfoTable.add_row({"Name", "Type", "#DV"});
  for (int i = 0; i < doc.GetColumnCount(); i++) {
    std::string colName = doc.GetColumnNames()[i];
    std::string sampleValue = doc.GetCell<std::string>(i, 0);
    std::string colType;
    if (isInt(sampleValue))
      colType = "Integer";
    else if (isFloat(sampleValue))
      colType = "Float";
    else
      colType = "Literal";

    std::vector<std::string> col = doc.GetColumn<std::string>(colName);
    std::sort(col.begin(), col.end());
    int dv = 1;
    for (int j = 1; j < col.size(); j++) {
      if (col[j] != col[j - 1])
        dv++;
    }

    columnInfoTable.add_row({colName, colType, std::to_string(dv)});
  }
  columnInfoTable.row(0).format().font_color(tabulate::Color::yellow);

  return columnInfoTable;
}

std::vector<int> Quantize(std::vector<float> const &val, float min, float max,
                          int bkt) {
  std::vector<int> ids;
  float width = (max - min) * 1.0 / bkt;
  for (auto it : val) {
    int id = std::trunc((it - min) * 1.0 / width);
    if (id >= bkt) id--;
    ids.push_back(id);
  }

  return ids;
}

std::vector<int> Quantize(std::vector<float> const &val,
                          std::unordered_map<float, int> lookup) {
  std::vector<int> ids;
  for (auto it : val) {
    ids.push_back(lookup[it]);
  }

  return ids;
}

int QuantizeQuery(float val, float min, float max, int bkt) {
  float width = (max - min) * 1.0 / bkt;
  return std::trunc((val - min) * 1.0 / width);
}

int QuantizeQuery(float val, std::unordered_map<float, int> lookup) {
  return lookup[val];
}

std::unordered_map<float, int> BuildLookup(std::vector<float> val) {
  std::sort(val.begin(), val.end());
  std::unordered_map<float, int> map;
  int id = 0;
  map[val[0]] = id;
  for (int i = 1; i < val.size(); i++) {
    if (val[i] != val[i - 1]) {
      id++;
      map[val[i]] = id;
    }
  }

  return map;
}

std::pair<float, float> GetQuery(std::vector<float> const &val) {
  float a = val[rand() % val.size()];
  float b = val[rand() % val.size()];
  return (a > b) ? std::pair<float, float>(b, a)
                 : std::pair<float, float>(a, b);
}

int BruteForce(std::vector<float> const &fst, std::vector<float> const &snd,
               std::pair<float, float> const &query_fst,
               std::pair<float, float> const &query_snd) {
  int cnt = 0;
  for (int i = 0; i < fst.size(); i++) {
    if (query_fst.first <= fst[i] && query_fst.second >= fst[i] &&
        query_snd.first <= snd[i] && query_snd.second >= snd[i])
      cnt++;
  }

  return cnt;
}

tabulate::Table DoExperiment(std::vector<std::string> const &fst,
                             std::vector<std::string> const &snd, int bkt_fst,
                             int bkt_snd, int dv_fst, int dv_snd) {
  tabulate::Table results;
  results.add_row({"Lower Bound", "Upper Bound", "Estimated Value",
                   "Actual Value", "Q-Error"});
  results.row(0).format().font_color(tabulate::Color::blue);

  float max_fst, min_fst, max_snd, min_snd;
  std::vector<float> val_fst;
  std::vector<float> val_snd;
  for (int i = 0; i < fst.size(); i++) {
    try {
      float tmp_fst = std::stof(fst[i]);
      float tmp_snd = std::stof(snd[i]);
      val_fst.push_back(tmp_fst);
      val_snd.push_back(tmp_snd);
      max_fst = (i == 0 || max_fst < tmp_fst) ? tmp_fst : max_fst;
      min_fst = (i == 0 || min_fst > tmp_fst) ? tmp_fst : min_fst;
      max_snd = (i == 0 || max_snd < tmp_snd) ? tmp_snd : max_snd;
      min_snd = (i == 0 || min_snd > tmp_snd) ? tmp_snd : min_snd;
    } catch (std::exception &err) {
      continue;
    }
  }

  std::unordered_map<float, int> lookup_fst, lookup_snd;
  std::vector<int> ids_fst, ids_snd;
  if (bkt_fst >= dv_fst) {
    lookup_fst = BuildLookup(val_fst);
    ids_fst = Quantize(val_fst, lookup_fst);
  } else {
    ids_fst = Quantize(val_fst, min_fst, max_fst, bkt_fst);
  }

  if (bkt_snd >= dv_snd) {
    lookup_snd = BuildLookup(val_snd);
    ids_snd = Quantize(val_snd, lookup_snd);
  } else {
    ids_snd = Quantize(val_snd, min_snd, max_snd, bkt_snd);
  }

  std::vector<std::vector<int>> dist(bkt_fst, std::vector<int>(bkt_snd, 0));
  for (int i = 0; i < ids_fst.size(); i++) {
    dist[ids_fst[i]][ids_snd[i]]++;
  }

  std::vector<std::vector<int>> prefix(bkt_fst, std::vector<int>(bkt_snd, 0));
  for (int i = 0; i < bkt_fst; i++) {
    for (int j = 0; j < bkt_snd; j++) {
      if (i == 0) {
        if (j == 0) {
          prefix[i][j] = dist[i][j];
        } else {
          prefix[i][j] = prefix[i][j - 1] + dist[i][j];
        }
      } else {
        if (j == 0) {
          prefix[i][j] = prefix[i - 1][j] + dist[i][j];
        } else {
          prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] -
                         prefix[i - 1][j - 1] + dist[i][j];
        }
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    std::pair<float, float> query_fst = GetQuery(val_fst);
    std::pair<float, float> query_snd = GetQuery(val_snd);
    int act = BruteForce(val_fst, val_snd, query_fst, query_snd);

    int lb_fst, ub_fst;
    if (bkt_fst >= dv_fst) {
      lb_fst = QuantizeQuery(query_fst.first, lookup_fst);
      ub_fst = QuantizeQuery(query_fst.second, lookup_fst);
    } else {
      lb_fst = QuantizeQuery(query_fst.first, min_fst, max_fst, bkt_fst);
      ub_fst = QuantizeQuery(query_fst.second, min_fst, max_fst, bkt_fst);
    }

    int lb_snd, ub_snd;
    if (bkt_snd >= dv_snd) {
      lb_snd = QuantizeQuery(query_snd.first, lookup_snd);
      ub_snd = QuantizeQuery(query_snd.second, lookup_snd);
    } else {
      lb_snd = QuantizeQuery(query_snd.first, min_snd, max_snd, bkt_snd);
      ub_snd = QuantizeQuery(query_snd.second, min_snd, max_snd, bkt_snd);
    }

    int est;
    if (lb_fst == 0) {
      if (lb_snd == 0) {
        est = prefix[ub_fst][ub_snd];
      } else {
        est = prefix[ub_fst][ub_snd] - prefix[ub_fst][lb_snd - 1];
      }
    } else {
      if (lb_snd == 0) {
        est = prefix[ub_fst][ub_snd] - prefix[lb_fst - 1][ub_snd];
      } else {
        est = prefix[ub_fst][ub_snd] - prefix[ub_fst][lb_snd - 1] -
              prefix[lb_fst - 1][ub_snd] + prefix[lb_fst - 1][lb_snd - 1];
      }
    }

    float qerr = std::max(est, act) * 1.0 / std::min(est, act);
    std::string lb = "(" + std::to_string(query_fst.first) + ", " + std::to_string(query_snd.first) + ")";
    std::string ub = "(" + std::to_string(query_fst.second) + ", " + std::to_string(query_snd.second) + ")";
    results.add_row({lb, ub, std::to_string(est), std::to_string(act), std::to_string(qerr)});
  }

  return results;
}