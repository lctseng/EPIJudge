#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <string>
#include <unordered_map>
#include <vector>
using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

struct Subarray {
  // Represent subarray by starting and ending indices, inclusive.
  int start, end;
};

Subarray
FindSmallestSequentiallyCoveringSubset(const vector<string> &paragraph,
                                       const vector<string> &keywords) {
  // start from small problem: small paragraph.
  // maintain a hash, key is keyword, value is the start_index of that seq in
  // paragraph, and it fulfill the constraint
  // when meet start word, create a new entry
  // when meet successor word, find from hash and update (remove and insert)
  //
  // ex: a,b,a,b,a,c , to find: b,a,c
  // after looking a,b,a,b the hash: a->1, b->3
  // after a,b,a,b,a, get new a->3, force replace old
  vector<int> startIndexMap(keywords.size(), -1); // key is keyword index
  unordered_map<string, int> wordToIndex; // map prev word for every keyword
  for (int i = 1; i < keywords.size(); i++) {
    wordToIndex[keywords[i]] = i;
  }
  // scan over paragraph
  Subarray minSubarray = {-1, -1};
  int minDistance = paragraph.size() + 1;
  for (int i = 0; i < paragraph.size(); i++) {
    auto &currentWord = paragraph[i];
    // start word check, add new start entry
    if (currentWord == keywords[0]) {
      startIndexMap[0] = i;
    }
    // preWord check
    else {
      auto wordIndexIt = wordToIndex.find(currentWord);
      if (wordIndexIt != wordToIndex.end()) {
        int kwIndex = wordIndexIt->second;
        int startIndex = startIndexMap[kwIndex - 1];
        if (startIndex != -1) { // if exist,  concat
          // remove old
          startIndexMap[kwIndex - 1] = -1;
          // if this word is the ending keyword
          // update minSubarray
          if (currentWord == keywords.back()) {
            int newDistance = i - startIndex;
            if (newDistance < minDistance) {
              minSubarray.start = startIndex;
              minSubarray.end = i;
              minDistance = newDistance;
            }
          } else {
            // otherwise, compute new entry and insert(overwrite)
            startIndexMap[kwIndex] = startIndex;
          }
        }
        // not found: cannot find a unused starting for this keyword
        // ignore
      }
      // not found: this word is not keyword
    }
  }
  // invalid words are ignored
  return minSubarray;
}
int FindSmallestSequentiallyCoveringSubsetWrapper(
    TimedExecutor &executor, const vector<string> &paragraph,
    const vector<string> &keywords) {
  auto result = executor.Run([&] {
    return FindSmallestSequentiallyCoveringSubset(paragraph, keywords);
  });

  int kw_idx = 0;
  if (result.start < 0) {
    throw TestFailure("Subarray start index is negative");
  }
  int para_idx = result.start;

  while (kw_idx < keywords.size()) {
    if (para_idx >= paragraph.size()) {
      throw TestFailure("Not all keywords are in the generated subarray");
    }
    if (para_idx >= paragraph.size()) {
      throw TestFailure("Subarray end index exceeds array size");
    }
    if (paragraph[para_idx] == keywords[kw_idx]) {
      kw_idx++;
    }
    para_idx++;
  }
  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "paragraph", "keywords"};
  return GenericTestMain(args, "smallest_subarray_covering_all_values.cc",
                         "smallest_subarray_covering_all_values.tsv",
                         &FindSmallestSequentiallyCoveringSubsetWrapper,
                         DefaultComparator{}, param_names);
}
