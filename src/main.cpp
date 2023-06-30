#include "rapidcsv.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/table.hpp>
#include <vector>

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

int main(int argc, char *argv[]) {
  rapidcsv::Document doc(argv[1]);

  std::cout << "Read file: " << argv[1] << std::endl;
  int cols = doc.GetColumnCount();
  int rows = doc.GetRowCount();
  std::cout << "Size(Cols * Rows): " << cols << "*" << rows << std::endl;
  std::cout << std::endl;

  std::cout << "Column Information:" << std::endl;
  tabulate::Table colInfoTable = GetColumnInfo(doc);
  std::cout << colInfoTable << std::endl;
  std::cout << std::endl;

  // Initialize random number
  srand((int)time(NULL));

  for (int i = 0; i < cols - 1; i++) {
    std::string type_fst = colInfoTable[i + 1][1].get_text();
    if (type_fst != "Literal") {
      for (int j = i + 1; j < cols; j++) {
        std::string type_snd = colInfoTable[j + 1][1].get_text();
        if (type_snd != "Literal") {
          std::cout << "Experiments on " << doc.GetColumnName(i) << " * "
                    << doc.GetColumnName(j) << std::endl;
          tabulate::Table expTable;
          expTable.add_row({"Bit Width", "Allocated Size", "Expected Size",
                            "Experiment Result"});
          expTable.row(0).format().font_color(tabulate::Color::yellow);
          int dv_fst = std::stoi(colInfoTable[i + 1][2].get_text());
          int dv_snd = std::stoi(colInfoTable[j + 1][2].get_text());
          std::string exp_size =
              std::to_string(dv_fst) + " * " + std::to_string(dv_snd);
          for (int bits = 8; bits <= 16; bits += 4) {
            int alloc1 = ComputeBuckets(dv_fst, dv_snd, bits);
            std::string alloc_size = std::to_string(int(pow(2, alloc1))) +
                                     " * " +
                                     std::to_string(int(pow(2, bits - alloc1)));
            std::vector<std::string> col_fst = doc.GetColumn<std::string>(i);
            std::vector<std::string> col_snd = doc.GetColumn<std::string>(j);
            expTable.add_row(
                {std::to_string(bits), alloc_size, exp_size,
                 DoExperiment(col_fst, col_snd, int(pow(2, alloc1)),
                              int(pow(2, bits - alloc1)), dv_fst, dv_snd)});
          }
          std::cout << expTable << std::endl;
          std::cout << std::endl;
        }
      }
    }
  }
}