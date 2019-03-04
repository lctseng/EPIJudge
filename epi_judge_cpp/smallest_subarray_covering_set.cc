#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct Subarray {
  int start, end;
};

Subarray
FindSmallestSubarrayCoveringSet(const vector<string> &paragraph,
                                const unordered_set<string> &keywords) {
  // i: first index meet sol, pass-over-i will not meet
  // j: past-over-last index meet sol
  // minDistance, minI, current found answer pair. finally return minI, minI + D
  // - 1 distance = j - i i = 0, j = 0 while in distance
  // --> j = i and advance J until all contains
  // --> advance I , shrink the range until cannot shrink anymore
  // --> force shrink i
  if (keywords.empty())
    return {0, 0};
  unordered_map<string, int> countMap;
  int i = 0, minI = 0, minDistance = paragraph.size(), j = 0;
  while (i < paragraph.size()) {
    while (j < paragraph.size() && countMap.size() < keywords.size()) {
      if (keywords.count(paragraph[j])) {
        // this is what we expect
        countMap[paragraph[j]]++;
      }
      j++;
    }
    // size match, we try shrink i
    while (i < j) {
      if (keywords.count(paragraph[i])) {
        auto &currentCount = countMap[paragraph[i]];
        if (currentCount > 1) {
          countMap[paragraph[i]] -= 1;
        } else {
          // we only have 1 this word, cannot advance anymore
          break;
        }
      } else {
        // this word is not important at all
      }
      i++;
    }
    // BE CAREFUL!
    // out-of-range i (i == j == size() - 1)
    if (countMap.size() == keywords.size()) {
      int currentD = j - i;
      if (currentD < minDistance) {
        minI = i;
        minDistance = currentD;
      }
      // force shrink I
      countMap.erase(paragraph[i]);
    }
    i++;
  }
  // BE CAREFUL!
  return {minI, minI + minDistance - 1}; // -1 since we need INCLUSIVE interval
}
int FindSmallestSubarrayCoveringSetWrapper(
    TimedExecutor &executor, const vector<string> &paragraph,
    const unordered_set<string> &keywords) {
  unordered_set<string> copy = keywords;

  auto result = executor.Run(
      [&] { return FindSmallestSubarrayCoveringSet(paragraph, keywords); });

  if (result.start < 0 || result.start >= paragraph.size() || result.end < 0 ||
      result.end >= paragraph.size() || result.start > result.end) {
    throw TestFailure("Index out of range");
  }

  for (int i = result.start; i <= result.end; i++) {
    copy.erase(paragraph[i]);
  }

  if (!copy.empty()) {
    throw TestFailure("Not all keywords are in the range");
  }

  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "paragraph", "keywords"};
  return GenericTestMain(args, "smallest_subarray_covering_set.cc",
                         "smallest_subarray_covering_set.tsv",
                         &FindSmallestSubarrayCoveringSetWrapper,
                         DefaultComparator{}, param_names);
}
