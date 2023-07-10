#include "rapidcsv.h"
#include "utils.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <tabulate/table.hpp>
#include <vector>

std::string GetFilename(std::string path) {
  std::stringstream ss(path);
  std::string str;

  while (std::getline(ss, str, '/'))
    ;

  std::stringstream sss(str);
  std::string filename;
  getline(sss, filename, '.');

  return filename;
}

int main(int argc, char *argv[]) {
  rapidcsv::Document doc(argv[1]);
  std::string filename = GetFilename(argv[1]);

  std::cout << "Read file: " << filename << ".csv" << std::endl;
  int cols = doc.GetColumnCount();
  int rows = doc.GetRowCount();
  std::cout << "Size(Cols * Rows): " << cols << "*" << rows << std::endl;
  std::cout << std::endl;

  // Print Column Information
  std::cout << "Column Information:" << std::endl;
  tabulate::Table colInfoTable = GetColumnInfo(doc);
  std::cout << colInfoTable << std::endl;
  std::cout << std::endl;

  std::ofstream result;
  std::string result_filename = "result_" + filename + ".md";
  result.open(result_filename);
  result << "# Result of " << filename << ".csv" << std::endl << std::endl;

  tabulate::MarkdownExporter exporter;
  auto md = exporter.dump(colInfoTable);
  result << "## Column information" << std::endl << std::endl;
  result << md << std::endl << std::endl;

  // Initialize random number
  srand((int)time(NULL));

  tabulate::Table summaryTable1;
  tabulate::Table summaryTable2;

  summaryTable1.add_row({"Column Set", "Bit Width", "#DV", "Allocated Buckets",
                         "Average Q-Error", "Maximum Q-Error",
                         "Minimum Q-Error", "Invalid Results"});

  summaryTable2.add_row({"Column Set", "Bit Width", "#DV", "Allocated Buckets",
                         "Average Q-Error", "Maximum Q-Error",
                         "Minimum Q-Error", "Invalid Results"});

  //result << "## Experiment results" << std::endl << std::endl;
  for (int i = 0; i < cols - 1; i++) {
    std::string type_fst = colInfoTable[i + 1][1].get_text();
    if (type_fst != "Literal") {
      for (int j = i + 1; j < cols; j++) {
        std::string type_snd = colInfoTable[j + 1][1].get_text();
        if (type_snd != "Literal") {
          std::string columnset_name =
              doc.GetColumnName(i) + " * " + doc.GetColumnName(j);
          std::cout << "Experiments on " << columnset_name << std::endl;
          //result << "### Experiments on " << columnset_name << std::endl
          //       << std::endl;
          int dv_fst = std::stoi(colInfoTable[i + 1][2].get_text());
          int dv_snd = std::stoi(colInfoTable[j + 1][2].get_text());
          std::vector<std::string> col_fst = doc.GetColumn<std::string>(i);
          std::vector<std::string> col_snd = doc.GetColumn<std::string>(j);

          DoExperiment(columnset_name, col_fst, col_snd, dv_fst, dv_snd,
                       summaryTable1, summaryTable2, result, exporter);
        }
      }
    }
  }

  std::cout << "Summary" << std::endl;
  std::cout << "Use naive method" << std::endl;
  std::cout << summaryTable2 << std::endl;
  std::cout << "Use uniform assumption" << std::endl;
  std::cout << summaryTable1 << std::endl;

  result << "## Summary" << std::endl << std::endl;
  result << "### Use naive method" << std::endl << std::endl;
  md = exporter.dump(summaryTable2);
  result << md << std::endl;

  result << "### Use uniform assumption" << std::endl << std::endl;
  md = exporter.dump(summaryTable1);
  result << md << std::endl;
  result.close();
}