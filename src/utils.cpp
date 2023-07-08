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

std::vector<float> BuildLookup(std::vector<float> val, int bkt, int dv) {
  // The estimated capacity of #DVs in one bucket
  float capacity = dv * 1.0 / bkt;
  capacity = (capacity <= 1.0) ? 1.0 : capacity;

  std::sort(val.begin(), val.end());
  std::vector<float> lookup;

  int cnt = 0;
  int id = 0;
  lookup.push_back(val[0]);
  for (int i = 1; i < val.size(); i++) {
    if (val[i] != val[i - 1]) {
      cnt++;
      if (std::trunc(cnt * 1.0 / capacity) != id) {
        id++;
        lookup.push_back(val[i]);
      }
    }
  }

  return lookup;
}

int FindID(std::vector<float> const &lookup, float key) {
  int left = 0;
  int right = lookup.size() - 1;

  while (left < right) {
    int mid = (left + right) / 2;
    if (mid == lookup.size())
      return mid;

    float tmp = lookup[mid];
    if (tmp <= key && key < lookup[mid + 1])
      return mid;

    if (tmp < key)
      left = mid + 1;
    else
      right = mid;
  }

  return left;
}

std::vector<int> Quantize(std::vector<float> const &val,
                          std::vector<float> const &lookup) {
  std::vector<int> ids;
  for (auto it : val) {
    ids.push_back(FindID(lookup, it));
  }

  return ids;
}

float QuantizeQuery(std::vector<float> const &lookup, float query, float max) {
  int id = FindID(lookup, query);
  float width = (id == lookup.size() - 1) ? max - lookup[id]
                                          : lookup[id + 1] - lookup[id];

  return id + (query - lookup[id]) / width;
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

int GetPrefixSum(std::vector<std::vector<int>> const &prefix, int x, int y) {
  if (x == 0 || y == 0)
    return 0;
  else
    return prefix[x - 1][y - 1];
}

float GetPrefixSum(std::vector<std::vector<int>> const &prefix, float x,
                   float y) {
  // Any float number can be expressed as Q + R, where Q is the integer part and
  // R is the remnant
  int qx = std::trunc(x);
  int qy = std::trunc(y);
  float rx = x - qx;
  float ry = y - qy;

  float ans = (1 - rx) * (1 - ry) * GetPrefixSum(prefix, qx, qy) +
              rx * (1 - ry) * GetPrefixSum(prefix, qx + 1, qy) +
              ry * (1 - rx) * GetPrefixSum(prefix, qx, qy + 1) +
              rx * ry * GetPrefixSum(prefix, qx + 1, qy + 1);

  // std::cout << "Queried: (" << x << ", " << y << ")" << std::endl;
  // std::cout << "Returned: " << ans << std::endl;
  return ans;
}

tabulate::Table DoExperiment(std::vector<std::string> const &fst,
                             std::vector<std::string> const &snd, int bkt_fst,
                             int bkt_snd, int dv_fst, int dv_snd) {

  float epsilon = EPSILON;
  
  tabulate::Table results;
  results.add_row({"Query Lower Bound", "Query Upper Bound", "Estimated Value",
                   "Actual Value", "Q-Error"});
  results.row(0).format().font_color(tabulate::Color::blue);

  // To filter unparsable values (i.e. '?' or NaN)
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

  // Quantize the column values
  auto lookup_fst = BuildLookup(val_fst, bkt_fst, dv_fst);
  auto lookup_snd = BuildLookup(val_snd, bkt_snd, dv_snd);
  auto ids_fst = Quantize(val_fst, lookup_fst);
  auto ids_snd = Quantize(val_snd, lookup_snd);

  // Generate the distribution matrix
  std::vector<std::vector<int>> dist(bkt_fst, std::vector<int>(bkt_snd, 0));
  for (int i = 0; i < ids_fst.size(); i++) {
    dist[ids_fst[i]][ids_snd[i]]++;
  }

  // Build up the prefix sum matrix
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

  // Randomly generate 10 queries
  for (int i = 0; i < 10; i++) {
    std::pair<float, float> query_fst = GetQuery(val_fst);
    std::pair<float, float> query_snd = GetQuery(val_snd);
    int act = BruteForce(val_fst, val_snd, query_fst, query_snd);

    // Quantize the given query
    float lb_fst = QuantizeQuery(lookup_fst, query_fst.first, max_fst);
    float ub_fst = QuantizeQuery(lookup_fst, query_fst.second, max_fst);
    ub_fst = (ub_fst - lb_fst > epsilon) ? ub_fst : lb_fst + epsilon;

    float lb_snd = QuantizeQuery(lookup_snd, query_snd.first, max_snd);
    float ub_snd = QuantizeQuery(lookup_snd, query_snd.second, max_snd);
    ub_snd = (ub_snd - lb_snd > epsilon) ? ub_snd : lb_snd + epsilon;

    // Use the prefix-sum matrix for CE
    int est = std::round(GetPrefixSum(prefix, ub_fst, ub_snd) -
                         GetPrefixSum(prefix, lb_fst, ub_snd) -
                         GetPrefixSum(prefix, ub_fst, lb_snd) +
                         GetPrefixSum(prefix, lb_fst, lb_snd));

    float qerr = std::max(est, act) * 1.0 / std::min(est, act);
    std::string lb = "(" + std::to_string(query_fst.first) + ", " +
                     std::to_string(query_snd.first) + ")";
    std::string ub = "(" + std::to_string(query_fst.second) + ", " +
                     std::to_string(query_snd.second) + ")";
    results.add_row({lb, ub, std::to_string(est), std::to_string(act),
                     std::to_string(qerr)});
  }

  return results;
}