#include "test_framework/generic_test.h"
#include <iterator>
#include <vector>
using std::make_pair;
using std::reverse;
using std::vector;
vector<int>
ExamineBuildingsWithSunset(vector<int>::const_iterator sequence_begin,
                           const vector<int>::const_iterator &sequence_end) {
  // monostack! strictly-decreasing stack
  vector<int> stk; // store index
  int index = 0;
  auto current_it = sequence_begin;
  while (current_it != sequence_end) {
    int current = *(current_it++);
    while (stk.size() && current >= *(sequence_begin + stk.back())) {
      stk.pop_back();
    }
    stk.push_back(index);
    index++;
  }
  reverse(stk.begin(), stk.end());
  return stk;
}
vector<int> ExamineBuildingsWithSunsetWrapper(const vector<int> &sequence) {
  return ExamineBuildingsWithSunset(cbegin(sequence), cend(sequence));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "sunset_view.cc", "sunset_view.tsv",
                         &ExamineBuildingsWithSunsetWrapper,
                         DefaultComparator{}, param_names);
}
