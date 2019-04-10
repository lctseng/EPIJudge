#include "test_framework/generic_test.h"
#include <climits>
#include <limits>
#include <string>
#include <vector>
using std::move;
using std::size;
using std::string;
using std::vector;

vector<int> cache;

int MinimumMessinessHelper(const vector<string> &words, int line_length,
                           int index) {
  if (index < 0)
    return 0;
  auto &cacheEntry = cache[index];
  if (cacheEntry >= 0)
    return cacheEntry;
  // consider placement of last word
  int spaceLeft = line_length;
  // it is garuanteed every word is shorter than line_length
  int minValue = std::numeric_limits<int>::max();
  int lastLineStartIndex = index;
  while (lastLineStartIndex >= 0) {
    // try use words[index]
    if ((lastLineStartIndex == index &&
         spaceLeft >= words[lastLineStartIndex].length()) ||
        spaceLeft > words[lastLineStartIndex].length()) {
      spaceLeft -= words[lastLineStartIndex].length();
      if (lastLineStartIndex < index)
        --spaceLeft;
      int currentMessiness =
          spaceLeft * spaceLeft +
          MinimumMessinessHelper(words, line_length, lastLineStartIndex - 1);
      if (currentMessiness < minValue) {
        minValue = currentMessiness;
      }
      --lastLineStartIndex;
    } else {
      // cannot fit more
      break;
    }
  }
  return cacheEntry = minValue;
}

int MinimumMessiness(const vector<string> &words, int line_length) {
  // dp[i]: min messiness considering words[0:i]
  // considering last word: single in last? two in last?
  // until cannot go futher
  cache.assign(words.size(), -1);
  return MinimumMessinessHelper(words, line_length, (int)words.size() - 1);
}

// Prev Backup
// vector<int> dp;

// int MinimumMessinessHelper(const vector<string> &words, int line_length,
//                            int index) {
//   if (index < 0)
//     return 0;
//   else if (dp[index] >= 0)
//     return dp[index];
//   // TODO: using up
//   // consider using words[index]
//   int j = index;
//   int minValue = INT_MAX;
//   int remainSpace = line_length;
//   while (j >= 0) {
//     // try words[j:index]
//     // update: add words[j]
//     if ((j == index && remainSpace >= size(words[j])) ||
//         (j < index && remainSpace > size(words[j]))) {
//       remainSpace -= size(words[j]);
//       if (j < index)
//         --remainSpace;
//       int currentTotal = remainSpace * remainSpace +
//                          MinimumMessinessHelper(words, line_length, j - 1);
//       if (currentTotal < minValue)
//         minValue = currentTotal;
//       // BE CAREFUL
//       // update j
//       --j;
//     } else {
//       // unable to fit words[j:index], stop
//       break;
//     }
//   }
//   return dp[index] = minValue;
// }

// int MinimumMessiness(const vector<string> &words, int line_length) {
//   // dp[i]: minimun messiness considering words[0:i]
//   // dp[i+1]: consider last line with only word[i+1], or word[j]~word[i+1],
//   // where j <= i
//   dp.clear();
//   dp.assign(words.size(), -1); // -1 for unknown
//   return MinimumMessinessHelper(words, line_length, size(words) - 1);
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "line_length"};
  return GenericTestMain(args, "pretty_printing.cc", "pretty_printing.tsv",
                         &MinimumMessiness, DefaultComparator{}, param_names);
}
