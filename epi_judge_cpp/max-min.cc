#include <iostream>
#include <vector>
using namespace std;

int MaxOfMin(const vector<int> &A, int K) {
  vector<vector<int>> dp(K + 1, vector<int>(A.size(), 0));
  vector<int> prefix;
  prefix.push_back(0);
  for (int val : A) {
    prefix.push_back(prefix.back() + val);
  }
  auto Sum = [&](int i, int j) { return prefix[j + 1] - prefix[i]; };
  // edge case: k is 1
  // for k= 1, just sum up all
  for (int i = 0; i < A.size(); i++) {
    dp[1][i] = Sum(0, i);
  }
  // dp, complexity is O(KN^2)
  for (int k = 2; k <= K; k++) {
    for (int i = k - 1; i < A.size(); i++) {
      // find number of last subarray
      for (int j = 1; j <= i; j++) {
        dp[k][i] = max(dp[k][i], min(dp[k - 1][i - j], Sum(i - j + 1, i)));
      }
    }
  }
  // print the dp table for reference
  for (int k = 0; k <= K; k++) {
    for (int i = 0; i < A.size(); i++) {
      cout << dp[k][i] << ' ';
    }
    cout << endl;
  }
  return dp[K][A.size() - 1];
}

int main() {
  int K = 3;
  vector<int> A = {1, 2, 3, 4, 5};
  cout << MaxOfMin(A, K) << endl;
  return 0;
}