#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::min;
using std::string;
using std::vector;

int LevenshteinDistanceHelper(vector<vector<int>> &dp, const string &A,
                              const string &B, int i, int j) {
  if (i >= A.length()) {
    // A is empty, distance is len(B)
    return B.length() - j;
  } else if (j >= B.length()) {
    return A.length() - i;
  } else {
    // A, B both exists
    int &dpEntry = dp[i][j];
    if (dpEntry >= 0)
      return dpEntry;
    if (A[i] == B[j]) {
      return dpEntry = LevenshteinDistanceHelper(dp, A, B, i + 1, j + 1);
    } else {
      return dpEntry = min(LevenshteinDistanceHelper(dp, A, B, i + 1, j + 1),
                           min(LevenshteinDistanceHelper(dp, A, B, i, j + 1),
                               LevenshteinDistanceHelper(dp, A, B, i + 1, j))) +
                       1;
    }
  }
}

int LevenshteinDistance(const string &A, const string &B) {
  vector<vector<int>> dp(A.length());
  for (int i = 0; i < A.length(); i++) {
    dp[i].resize(B.length(), -1);
  }
  return LevenshteinDistanceHelper(dp, A, B, 0, 0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
