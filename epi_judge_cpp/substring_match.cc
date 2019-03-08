#include "test_framework/generic_test.h"
#include <string>
using std::move;
using std::string;
using std::vector;

// Returns the index of the first character of the substring if found, -1
// otherwise.

bool IsMatch(const string &haystack, const string &needle, int begin) {
  for (int i = 0; i < needle.size(); i++) {
    int index = begin + i;
    if (index >= haystack.length() || haystack[index] != needle[i])
      return false;
  }
  return true;
}

int RabinKarp(const string &haystack, const string &needle) {
  // edge case
  if (needle.length() > haystack.length())
    return -1;
  // hash function: use base26 number
  // step 1 compute hash for haystack and needle
  const int BASE = 26;
  // dont care for overflow
  unsigned hashHaystack = 0;
  unsigned hashNeedle = 0;
  unsigned power = 1; // use to rolliing update haystack
  for (int i = 0; i < needle.length(); i++) {
    if (i > 0)
      power = power * 26;
    hashHaystack = hashHaystack * BASE + haystack[i];
    hashNeedle = hashNeedle * BASE + needle[i];
  }
  for (int i = needle.length(); i < haystack.length(); i++) {
    // compare first
    if (hashHaystack == hashNeedle &&
        IsMatch(haystack, needle, i - needle.length())) {
      return i - needle.length();
    }
    // update haystack hash later
    // remove existing
    hashHaystack -= haystack[i - needle.length()] * power;
    hashHaystack = hashHaystack * BASE + haystack[i];
  }
  // match final
  if (hashHaystack == hashNeedle &&
      IsMatch(haystack, needle, haystack.length() - needle.length())) {
    return haystack.length() - needle.length();
  }
  // not found
  return -1;
}

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

int KMP(const string &haystack, const string &needle) {
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
