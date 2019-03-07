#include "test_framework/generic_test.h"
#include <climits>
#include <string>
#include <vector>
using std::move;
using std::size;
using std::string;
using std::vector;

vector<int> dp;

int MinimumMessinessHelper(const vector<string> &words, int line_length,
                           int index) {
  if (index < 0)
    return 0;
  else if (dp[index] >= 0)
    return dp[index];
  // TODO: using up
  // consider using words[index]
  int j = index;
  int minValue = INT_MAX;
  int remainSpace = line_length;
  while (j >= 0) {
    // try words[j:index]
    // update: add words[j]
    if ((j == index && remainSpace >= size(words[j])) ||
        (j < index && remainSpace > size(words[j]))) {
      remainSpace -= size(words[j]);
      if (j < index)
        --remainSpace;
      int currentTotal = remainSpace * remainSpace +
                         MinimumMessinessHelper(words, line_length, j - 1);
      if (currentTotal < minValue)
        minValue = currentTotal;
      // BE CAREFUL
      // update j
      --j;
    } else {
      // unable to fit words[j:index], stop
      break;
    }
  }
  return dp[index] = minValue;
}

int MinimumMessiness(const vector<string> &words, int line_length) {
  // dp[i]: minimun messiness considering words[0:i]
  // dp[i+1]: consider last line with only word[i+1], or word[j]~word[i+1],
  // where j <= i
  dp.clear();
  dp.assign(words.size(), -1); // -1 for unknown
  return MinimumMessinessHelper(words, line_length, size(words) - 1);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "line_length"};
  return GenericTestMain(args, "pretty_printing.cc", "pretty_printing.tsv",
                         &MinimumMessiness, DefaultComparator{}, param_names);
}
