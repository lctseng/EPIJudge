#include "test_framework/generic_test.h"
#include <unordered_map>
#include <vector>
using std::unordered_map;
using std::vector;

unordered_map<uint64_t, int> cache;

int NumCombinationsForFinalScoreHelper(int final_score,
                                       const vector<int> &scores, int index) {
  if (index >= scores.size())
    return final_score == 0;
  uint64_t key = ((uint64_t)final_score << 32) | index;
  auto it = cache.find(key);
  if (it != cache.end())
    return it->second;
  // choose this or not?
  // choose
  int count = 0;
  if (final_score >= scores[index]) {
    count += NumCombinationsForFinalScoreHelper(final_score - scores[index],
                                                scores, index);
  }
  // not choose
  count += NumCombinationsForFinalScoreHelper(final_score, scores, index + 1);
  return cache[key] = count;
}

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int> &individual_play_scores) {
  cache.clear();
  return NumCombinationsForFinalScoreHelper(final_score, individual_play_scores,
                                            0);
}

// Prev backup
// int NumCombinationsForFinalScoreTopDown(
//     vector<vector<int>> &memo, int final_score,
//     const vector<int> &individual_play_scores, int index) {
//   if (index == 0)
//     return final_score % individual_play_scores[0] == 0;
//   else if (final_score == 0)
//     return 1;
//   else if (final_score < 0) {
//     return 0;
//   }
//   int &memoEntry = memo[index][final_score];
//   if (memoEntry >= 0) {
//     return memoEntry;
//   } else {
//     // compute
//     int withoutThis = NumCombinationsForFinalScoreTopDown(
//         memo, final_score, individual_play_scores, index - 1);
//     int withThis = NumCombinationsForFinalScoreTopDown(
//         memo, final_score - individual_play_scores[index],
//         individual_play_scores, index);
//     return memoEntry = withoutThis + withThis;
//   }
// }

// int NumCombinationsForFinalScoreTopDown(
//     int final_score, const vector<int> &individual_play_scores) {
//   vector<vector<int>> memo(individual_play_scores.size());
//   for (int i = 0; i < individual_play_scores.size(); i++) {
//     memo[i].resize(final_score + 1, -1);
//   }
//   return NumCombinationsForFinalScoreTopDown(memo, final_score,
//                                              individual_play_scores,
//                                              individual_play_scores.size() -
//                                              1);
// }

// int NumCombinationsForFinalScoreBottomUp(
//     int final_score, const vector<int> &individual_play_scores) {
//   // bottom-up
//   vector<int> dp(final_score + 1);
//   dp[0] = 1;
//   // fill table
//   for (int i = 0; i < individual_play_scores.size(); i++) {
//     for (int j = 1; j <= final_score; j++) {
//       int scorePrev = j - individual_play_scores[i];
//       if (scorePrev >= 0) {
//         dp[j] += dp[scorePrev];
//       }
//     }
//   }
//   return dp[final_score];
// }

// int NumCombinationsForFinalScore(int final_score,
//                                  const vector<int> &individual_play_scores) {
//   return NumCombinationsForFinalScoreBottomUp(final_score,
//                                               individual_play_scores);
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
