#include "test_framework/generic_test.h"
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
using std::size;
using std::string;
using std::vector;

enum class Result { kTrue, kFalse, kNone };

vector<vector<vector<string>>> resCache;
vector<bool> resValid;
vector<vector<Result>> pCache;

bool IsPalindrome(const string &input, int begin, int end) {
  auto &cacheEntry = pCache[begin][end];
  if (cacheEntry != Result::kNone)
    return cacheEntry == Result::kTrue;
  int sz = end - begin;
  int halfSz = sz >> 1;
  for (int i = 0; i < halfSz; i++) {
    if (input[begin + i] != input[end - 1 - i]) {
      cacheEntry = Result::kFalse;
      return false;
    }
  }
  cacheEntry = Result::kTrue;
  return true;
}

const vector<vector<string>> &
PalindromeDecompositionsHelper(const string &input, int index) {
  if (resValid[index]) {
    return resCache[index];
  } else {
    resValid[index] = true;
    auto &res = resCache[index];
    // start from index, pick a split point
    for (int end = index + 1; end <= size(input); end++) {
      if (IsPalindrome(input, index, end)) {
        auto current = input.substr(index, end - index);
        const auto &subResults = PalindromeDecompositionsHelper(input, end);
        for (const auto &subResult : subResults) {
          vector<string> currentResult;
          currentResult.push_back(current);
          for (const auto &s : subResult) {
            currentResult.push_back(s);
          }
          res.push_back(move(currentResult));
        }
      }
    }
    return res;
  }
}

vector<vector<string>> PalindromeDecompositions(const string &input) {
  // TODO - you fill in here.
  resCache.assign(size(input) + 1, {});
  resValid.assign(size(input) + 1, false);
  resCache[size(input)].push_back({});
  resValid[size(input)] = true;

  pCache.assign(size(input) + 1, {});
  for (int i = 0; i <= size(input); i++) {
    pCache[i].assign(size(input) + 1, Result::kNone);
  };
  return PalindromeDecompositionsHelper(input, 0);
}
bool Comp(vector<vector<string>> expected, vector<vector<string>> result) {
  std::sort(begin(expected), end(expected));
  std::sort(begin(result), end(result));
  return expected == result;
};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input"};
  return GenericTestMain(args, "enumerate_palindromic_decompositions.cc",
                         "enumerate_palindromic_decompositions.tsv",
                         &PalindromeDecompositions, &Comp, param_names);
}
