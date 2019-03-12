#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int CalculateBonus(const vector<int> &productivity) {
  int current = 0;
  int total = 0;
  int lastPeakIndex = -1;
  int peakUsed = 0;
  for (int i = 0; i < productivity.size(); i++) {
    // find the peak
    if ((i == 0 || productivity[i] > productivity[i - 1]) &&
        (i == productivity.size() - 1 ||
         productivity[i] >= productivity[i + 1])) {
      // this is a peak
      current++;
      total += current;
      lastPeakIndex = i;
      peakUsed = current;
    } else if (i > 0 && productivity[i - 1] > productivity[i]) {
      // this is downward
      if (current == 1) {
        // previous is already take 1, and this i need to take less than it
        // need to pad until last peak!
        total += i - lastPeakIndex;
        // but if this gain more than peak got, also need to increase peak
        if (i - lastPeakIndex >= peakUsed) {
          ++peakUsed;
          ++total;
        }
      } else {
        // previous not 1, so this got 1
        current = 1;
        total++;
      }
    } else if (i > 0 && productivity[i - 1] == productivity[i]) {
      // same as previous
      // being new peak with 1
      peakUsed = 1;
      current = 1;
      lastPeakIndex = i;
      ++total;
    } else {
      // this is upwarding
      current++;
      total += current;
    }
  }
  return total;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"productivity"};
  return GenericTestMain(args, "bonus.cc", "bonus.tsv", &CalculateBonus,
                         DefaultComparator{}, param_names);
}
