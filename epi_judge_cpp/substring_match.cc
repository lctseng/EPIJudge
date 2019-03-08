#include "test_framework/generic_test.h"
#include <string>
using std::move;
using std::string;
using std::vector;

// Returns the index of the first character of the substring if found, -1
// otherwise.

vector<int> ComputeFailure(const string &needle) {
  vector<int> failure(needle.length());
  failure[0] = -1;
  for (int i = 1; i < needle.length(); i++) {
    int j = failure[i - 1];
    while (j >= 0 && needle[i] != needle[j + 1])
      j = failure[j];
    if (needle[i] == needle[j + 1]) {
      failure[i] = j + 1;
    } else {
      failure[i] = -1;
    }
  }
  return move(failure);
}

int RabinKarp(const string &haystack, const string &needle) {
  if (needle.empty())
    return 0;
  auto failure = ComputeFailure(needle);
  int s = 0, t = 0;
  while (s < haystack.length() && t < needle.length()) {
    if (haystack[s] == needle[t]) {
      ++s;
      ++t;
    } else {
      if (t == 0) {
        ++s;
      } else {
        t = failure[t - 1] + 1;
      }
    }
  }
  if (t == needle.length()) {
    return s - needle.length();
  } else {
    return -1;
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"t", "s"};
  return GenericTestMain(args, "substring_match.cc", "substring_match.tsv",
                         &RabinKarp, DefaultComparator{}, param_names);
}
