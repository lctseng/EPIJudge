#include "test_framework/generic_test.h"
#include <string>
#include <unordered_map>
#include <vector>
using std::min;
using std::string;
using std::unordered_map;
using std::vector;

int FindNearestRepetition(const vector<string> &paragraph) {
  unordered_map<string, int> seen;
  int minValue = paragraph.size();
  for (int i = 0; i < paragraph.size(); i++) {
    auto it = seen.find(paragraph[i]);
    if (it != seen.end()) {
      int distance = i - it->second;
      if (distance < minValue)
        minValue = distance;
    }
    seen[paragraph[i]] = i;
  }
  return minValue == paragraph.size() ? -1 : minValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"paragraph"};
  return GenericTestMain(args, "nearest_repeated_entries.cc",
                         "nearest_repeated_entries.tsv", &FindNearestRepetition,
                         DefaultComparator{}, param_names);
}
