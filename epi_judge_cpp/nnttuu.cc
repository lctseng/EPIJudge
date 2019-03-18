#include <iostream>
#include <string>
#include <vector>
using namespace std;

// dp[i][j] = number of combination for s[:i], t[:j]
//  if s[i] != t[j]
//     dp[i][j] = dp[i-1][j]
//  if s[i] == t[j]
//     dp[i][j] = dp[i-1][j-1] + dp[i-1][j]

// terminal:
// dp[i][0]: number of t[0] appear in s[:i], (include dp[0][0])
// dp[0][j] for j > 0, 0
vector<vector<int>> dp;

int CombinationCount(const string &s, const string &t, int i, int j) {
  if (dp[i][j] >= 0)
    return dp[i][j];
  if (s[i] != t[j]) {
    return dp[i][j] = CombinationCount(s, t, i - 1, j);
  } else {
    return dp[i][j] = CombinationCount(s, t, i - 1, j) +
                      CombinationCount(s, t, i - 1, j - 1);
  }
}

int CombinationCount(const string &s, const string &t) {
  dp.assign(s.length(), vector<int>(t.length(), -1));
  // fill terminal condition
  int count = 0;
  for (int i = 0; i < s.length(); i++) {
    if (t[0] == s[i])
      ++count;
    dp[i][0] = count;
  }
  for (int j = 1; j < t.length(); j++) {
    dp[0][j] = 0;
  }
  return CombinationCount(s, t, s.length() - 1, t.length() - 1);
}

int main() {
  cout << CombinationCount("nnttuu", "ntu") << endl;  // => 8
  cout << CombinationCount("ntnttuu", "ntu") << endl; // => 10
  return 0;
}