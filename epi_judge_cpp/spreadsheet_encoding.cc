#include "test_framework/generic_test.h"
#include <string>
using std::string;

int SSDecodeColID(const string &col) {
  int val = 0;
  for (char c : col) {
    val = val * 26 + (c - 'A' + 1);
  }
  return val;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"col"};
  return GenericTestMain(args, "spreadsheet_encoding.cc",
                         "spreadsheet_encoding.tsv", &SSDecodeColID,
                         DefaultComparator{}, param_names);
}
