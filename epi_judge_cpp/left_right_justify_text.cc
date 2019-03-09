#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::min;
using std::move;
using std::string;
using std::vector;

vector<string> JustifyText(const vector<string> &words, int L) {
  // process line by line
  // each line is indepedent
  // need special handling for one-word line
  int i = 0;
  vector<string> res;
  while (i < words.size()) {
    // iterate all words
    // for this line, try enclose as many word as possible
    // use index j. j is the last word can fit in current line
    // assume every word can fit in the line
    int j = i;
    // BE CAREFUL!
    // the remainSpace definition: remaining EXTRA spaces!
    // the mininum spacing is excluded!
    int remainingSpace = L;
    string builder;
    while (j < words.size()) {
      int spaceNeed = words[j].length() + (j > i);
      if (remainingSpace >= spaceNeed) {
        remainingSpace -= spaceNeed;
        j++;
      } else {
        break;
      }
    }
    --j;
    bool isLastLine = j == words.size() - 1;
    if (i == j) {
      // single word case
      builder.append(words[i]);
      builder.append(remainingSpace, ' ');
    } else {
      // compute max per place
      // Note: remaining space is already substract the basic usage!
      int numInterval = j - i;
      // BE CAREFUL!
      // Need to compute max/min Extra for future use!
      int maxExtraSpace, minExtraSpace;
      if (isLastLine) {
        maxExtraSpace = 0;
        minExtraSpace = 0;
      } else {
        maxExtraSpace = ceil((double)remainingSpace / numInterval);
        minExtraSpace = remainingSpace / numInterval;
      }
      // place words from i to j, inclusive
      for (int k = i; k < j; k++) {
        // place word
        builder.append(words[k]);
        // the remainingSpace are all extra
        // need to ensure remaining interval has enough minExtraSpace
        int remainInterval = j - k - 1;
        int canUseExtraSpaces = remainingSpace - remainInterval * minExtraSpace;
        int extraSpaceCount = min(canUseExtraSpaces, maxExtraSpace);
        builder.append(extraSpaceCount + 1, ' ');
        remainingSpace -= extraSpaceCount;
      }
      // last word
      builder.append(words[j]);
      // BE CAREFUL!
      // remainingSpace must > 0, or it throws exception
      if (isLastLine && remainingSpace > 0) {
        builder.append(remainingSpace, ' ');
      }
    }
    // push into result
    res.push_back(move(builder));
    // i = j + 1
    i = j + 1;
  }
  return move(res);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"words", "L"};
  return GenericTestMain(args, "left_right_justify_text.cc",
                         "left_right_justify_text.tsv", &JustifyText,
                         DefaultComparator{}, param_names);
}
