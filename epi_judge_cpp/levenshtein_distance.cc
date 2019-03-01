#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::min;
using std::string;
using std::swap;
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

int LevenshteinDistance(const string &_A, const string &_B) {
  const string *longPtr = &_A, *shortPtr = &_B;
  if (longPtr->length() < shortPtr->length()) {
    swap(longPtr, shortPtr);
  }
  const string &A = *longPtr;
  const string &B = *shortPtr;
  // A loop times, space = O(len(B))
  // total time: O(len(A) * len(B))
  vector<int> dp0(B.length() + 1), dp1(B.length() + 1);
  vector<int> *prevDp = &dp0, *currDp = &dp1;
  for (int j = 0; j <= B.length(); j++) {
    prevDp->at(j) = j;
  }
  // fill table
  for (int i = 1; i <= A.length(); i++) {
    currDp->at(0) = i; // BE CAREFUL! current dp init condition!
    for (int j = 1; j <= B.length(); j++) {
      if (A[i - 1] == B[j - 1]) {
        currDp->at(j) = prevDp->at(j - 1);
      } else {
        currDp->at(j) =
            min(prevDp->at(j - 1), min(prevDp->at(j), currDp->at(j - 1))) + 1;
      }
    }
    swap(currDp, prevDp);
  }
  return prevDp->at(B.length());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
