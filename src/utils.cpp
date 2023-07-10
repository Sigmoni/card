#include "utils.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <tabulate/table.hpp>
#include <tabulate/table_internal.hpp>
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

float Method1(std::vector<float> const &lookup_fst,
              std::vector<float> const &lookup_snd,
              std::pair<float, float> const &query_fst,
              std::pair<float, float> const &query_snd, float max_fst,
              float max_snd, int bkt_fst, int bkt_snd, int dv_fst, int dv_snd,
              std::vector<std::vector<int>> const &prefix, int act,
              tabulate::Table &results) {
  // Quantize the given query
  float lb_fst = QuantizeQuery(lookup_fst, query_fst.first, max_fst);
  float ub_fst = QuantizeQuery(lookup_fst, query_fst.second, max_fst);
  float epsilon = bkt_fst * 1.0 / dv_fst;
  epsilon = (epsilon > 1) ? 1 : epsilon;
  // ub_fst = (ub_fst - lb_fst > epsilon) ? ub_fst : lb_fst + epsilon;
  ub_fst += epsilon;

  float lb_snd = QuantizeQuery(lookup_snd, query_snd.first, max_snd);
  float ub_snd = QuantizeQuery(lookup_snd, query_snd.second, max_snd);
  epsilon = bkt_snd * 1.0 / dv_snd;
  epsilon = (epsilon > 1) ? 1 : epsilon;
  // ub_snd = (ub_snd - lb_snd > epsilon) ? ub_snd : lb_snd + epsilon;
  ub_snd += epsilon;

  int est = std::round(GetPrefixSum(prefix, ub_fst, ub_snd) -
                       GetPrefixSum(prefix, lb_fst, ub_snd) -
                       GetPrefixSum(prefix, ub_fst, lb_snd) +
                       GetPrefixSum(prefix, lb_fst, lb_snd));

  float qerr = std::max(est, act) * 1.0 / std::min(est, act);
  // std::string lb = "(" + std::to_string(query_fst.first) + ", " +
  //                  std::to_string(query_snd.first) + ")";
  // std::string qlb =
  //     "(" + std::to_string(lb_fst) + ", " + std::to_string(lb_snd) + ")";
  // std::string ub = "(" + std::to_string(query_fst.second) + ", " +
  //                  std::to_string(query_snd.second) + ")";
  // std::string qub =
  //     "(" + std::to_string(ub_fst) + ", " + std::to_string(ub_snd) + ")";
  // results.add_row({lb, qlb, ub, qub, std::to_string(est), std::to_string(act),
  //                  std::to_string(qerr)});

  return qerr;
}

float Method2(std::vector<float> const &lookup_fst,
              std::vector<float> const &lookup_snd,
              std::pair<float, float> const &query_fst,
              std::pair<float, float> const &query_snd, float max_fst,
              float max_snd, int bkt_fst, int bkt_snd, int dv_fst, int dv_snd,
              std::vector<std::vector<int>> const &prefix, int act,
              tabulate::Table &results) {
  int lb_fst = FindID(lookup_fst, query_fst.first);
  int ub_fst = FindID(lookup_fst, query_fst.second) + 1;
  int lb_snd = FindID(lookup_snd, query_snd.first);
  int ub_snd = FindID(lookup_snd, query_snd.second) + 1;

  // Use the prefix-sum matrix for CE
  int est = std::round(GetPrefixSum(prefix, ub_fst, ub_snd) -
                       GetPrefixSum(prefix, lb_fst, ub_snd) -
                       GetPrefixSum(prefix, ub_fst, lb_snd) +
                       GetPrefixSum(prefix, lb_fst, lb_snd));

  float qerr = std::max(est, act) * 1.0 / std::min(est, act);
  // std::string lb = "(" + std::to_string(query_fst.first) + ", " +
  //                  std::to_string(query_snd.first) + ")";
  // std::string qlb =
  //     "(" + std::to_string(lb_fst) + ", " + std::to_string(lb_snd) + ")";
  // std::string ub = "(" + std::to_string(query_fst.second) + ", " +
  //                  std::to_string(query_snd.second) + ")";
  // std::string qub =
  //     "(" + std::to_string(ub_fst) + ", " + std::to_string(ub_snd) + ")";
  // results.add_row({lb, qlb, ub, qub, std::to_string(est), std::to_string(act),
  //                  std::to_string(qerr)});

  return qerr;
}

int ComputeBuckets(int dv1, int dv2, int bits) {
  int alloc1 = 0;
  int alloc2 = 0;
  int bkt1 = 1;
  int bkt2 = 1;
  auto acc = [bkt1, bkt2, dv1, dv2]() {
    float acc1 = (bkt1 > dv1) ? 1.0 : 1 - 0.5 / bkt1;
    float acc2 = (bkt2 > dv2) ? 1.0 : 1 - 0.5 / bkt2;

    return acc1 * acc2;
  };
  float max = acc();

  for (int i = 0; i < bits; i++) {
    if (dv1 * 1.0 / bkt1 > dv2 * 1.0 / bkt2) {
      alloc1++;
      bkt1 *= 2;
    } else {
      alloc2++;
      bkt2 *= 2;
    }
  }

  return alloc1;
}

void DoExperiment(std::string columnset, std::vector<std::string> const &fst,
                  std::vector<std::string> const &snd, int dv_fst, int dv_snd,
                  tabulate::Table &summary1, tabulate::Table &summary2,
                  std::ofstream &result, tabulate::MarkdownExporter &exporter) {

  std::string exp_size =
      std::to_string(dv_fst) + " * " + std::to_string(dv_snd);
  std::cout << "#DV : " << exp_size << std::endl << std::endl;

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

  // Randomly generate #QUERY_WORKLOAD queries
  std::vector<Query> queries_fst;
  std::vector<Query> queries_snd;
  std::vector<int> act_vals;
  for (int i = 0; i < QUERY_WORKLOAD; i++) {
    Query query_fst = GetQuery(val_fst);
    Query query_snd = GetQuery(val_snd);
    int act = BruteForce(val_fst, val_snd, query_fst, query_snd);

    queries_fst.push_back(query_fst);
    queries_snd.push_back(query_snd);
    act_vals.push_back(act);
  }

  for (int bits = 8; bits <= 16; bits += 4) {
    tabulate::Table resultTable;
    resultTable.add_row({"Query Lower Bound", "Quantized", "Query Upper Bound",
                         "Quantized", "Estimated Value", "Actual Value",
                         "Q-Error"});
    resultTable.row(0).format().font_color(tabulate::Color::blue);

    int alloc_fst = ComputeBuckets(dv_fst, dv_snd, bits);
    int alloc_snd = bits - alloc_fst;
    int bkt_fst = std::pow(2, alloc_fst);
    int bkt_snd = std::pow(2, alloc_snd);
    std::string alloc_size =
        std::to_string(bkt_fst) + " * " + std::to_string(bkt_snd);

    std::cout << "Bit width : " << bits << std::endl;
    std::cout << "Allocate Buckets : " << alloc_size << std::endl << std::endl;

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

    // Do the queries
    float avg_qerr1 = 0;
    float max_qerr1 = 0;
    float min_qerr1 = 0;
    int cnt1 = QUERY_WORKLOAD;
    float avg_qerr2 = 0;
    float max_qerr2 = 0;
    float min_qerr2 = 0;
    int cnt2 = QUERY_WORKLOAD;
    for (int i = 0; i < QUERY_WORKLOAD; i++) {
      Query query_fst = queries_fst[i];
      Query query_snd = queries_snd[i];
      int act = act_vals[i];

      float qerr1 = Method1(lookup_fst, lookup_snd, query_fst, query_snd,
                            max_fst, max_snd, bkt_fst, bkt_snd, dv_fst, dv_snd,
                            prefix, act, resultTable);
      if (std::isfinite(qerr1)) {
        avg_qerr1 += qerr1;
        max_qerr1 = std::max(max_qerr1, qerr1);
        min_qerr1 = (i == 0) ? qerr1 : std::min(min_qerr1, qerr1);
      } else {
        cnt1--;
      }

      float qerr2 = Method2(lookup_fst, lookup_snd, query_fst, query_snd,
                            max_fst, max_snd, bkt_fst, bkt_snd, dv_fst, dv_snd,
                            prefix, act, resultTable);
      if (std::isfinite(qerr2)) {
        avg_qerr2 += qerr2;
        max_qerr2 = std::max(max_qerr2, qerr2);
        min_qerr2 = (i == 0) ? qerr2 : std::min(min_qerr2, qerr2);
      } else {
        cnt2--;
      }
    }
    avg_qerr1 /= cnt1;
    avg_qerr2 /= cnt2;

    //result << "#### Bit Width=" << bits << std::endl << std::endl;
    //result << "* Allocated Size: " << alloc_size << std::endl;
    //result << "* Expected Size: " << exp_size << std::endl;
    //result << "* Experiment Result:" << std::endl << std::endl;

    //std::string md = exporter.dump(resultTable);
    //result << md << std::endl << std::endl;

    summary1.add_row({columnset, std::to_string(bits), exp_size, alloc_size,
                      std::to_string(avg_qerr1), std::to_string(max_qerr1),
                      std::to_string(min_qerr1),
                      std::to_string(QUERY_WORKLOAD - cnt1)});

    summary2.add_row({columnset, std::to_string(bits), exp_size, alloc_size,
                      std::to_string(avg_qerr2), std::to_string(max_qerr2),
                      std::to_string(min_qerr2),
                      std::to_string(QUERY_WORKLOAD - cnt2)});

    std::cout << "-- Using naive methods:" << std::endl;
    std::cout << "   * Average Q-Error: " << avg_qerr2 << std::endl;
    std::cout << "   * Max Q-Error: " << max_qerr2 << std::endl;
    std::cout << "   * Min Q-Error: " << min_qerr2 << std::endl;
    std::cout << "   * Invalid Results: " << QUERY_WORKLOAD - cnt1 << std::endl;
    std::cout << std::endl;

    std::cout << "-- Using uniform assumption:" << std::endl;
    std::cout << "   * Average Q-Error: " << avg_qerr1 << std::endl;
    std::cout << "   * Max Q-Error: " << max_qerr1 << std::endl;
    std::cout << "   * Min Q-Error: " << min_qerr1 << std::endl;
    std::cout << "   * Invalid Results: " << QUERY_WORKLOAD - cnt1 << std::endl;
    std::cout << std::endl;
  }
}