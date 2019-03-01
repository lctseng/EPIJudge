#include "test_framework/generic_test.h"
#include <unordered_map>
#include <vector>
using std::unordered_map;
using std::vector;

unordered_map<long long, int> cache;

int NumCombinationsForFinalScoreHelper(
    int final_score, const vector<int> &individual_play_scores, int index) {
  if (index == individual_play_scores.size() - 1) {
    return final_score % individual_play_scores[index] == 0;
  } else {
    long long key = final_score;
    key <<= 32;
    key |= index;
    auto it = cache.find(key);
    if (it != cache.end())
      return it->second;
    int totalCount = 0;
    int thisScore = 0;
    while (thisScore <= final_score) {
      totalCount += NumCombinationsForFinalScoreHelper(
          final_score - thisScore, individual_play_scores, index + 1);
      thisScore += individual_play_scores[index];
    }
    return cache[key] = totalCount;
  }
}

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int> &individual_play_scores) {
  cache.clear();
  return NumCombinationsForFinalScoreHelper(final_score, individual_play_scores,
                                            0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
