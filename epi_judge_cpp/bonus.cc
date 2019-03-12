#include "test_framework/generic_test.h"
#include <vector>
using std::accumulate;
using std::max;
using std::vector;

int CalculateBonus(const vector<int> &productivity) {
  // simple solution but space is O(n)
  // scan from left to right, make sure increasing order holds
  int sz = productivity.size();
  vector<int> tickets(sz, 1);
  for (int i = 1; i < sz; i++) {
    if (productivity[i] > productivity[i - 1]) {
      tickets[i] = tickets[i - 1] + 1;
    }
  }
  // right to left
  for (int i = sz - 2; i >= 0; i--) {
    if (productivity[i] > productivity[i + 1]) {
      tickets[i] = max(tickets[i], tickets[i + 1] + 1);
    }
  }
  return accumulate(tickets.begin(), tickets.end(), 0);
}

// Fastest! One pass and O(1) space
int CalculateBonusConstantSpace(const vector<int> &productivity) {
  int current = 0;
  int total = 0;
  int lastPeakIndex = -1;
  int peakUsed = 0;
  for (int i = 0; i < productivity.size(); i++) {
    // find the peak
    if ((i == 0 || productivity[i] > productivity[i - 1]) &&
        (i == productivity.size() - 1 ||
         productivity[i] >= productivity[i + 1])) {
      // this is a first-half strictly increasing peak
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
