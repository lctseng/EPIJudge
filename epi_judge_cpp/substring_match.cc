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
  if (haystack.length() < needle.length())
    return -1;
  if (needle.empty())
    return 0;
  // now length of haystack is >= needle
  // use base 26 unsigned to compute the hash
  // compute hash for needle and first part of haystack
  // and compute the MSB power
  const unsigned BASE = 26;
  unsigned power = 1;
  unsigned hashHaystack = 0, hashNeedle = 0;
  for (int i = 0; i < needle.length(); i++) {
    hashHaystack = hashHaystack * BASE + haystack[i];
    hashNeedle = hashNeedle * BASE + needle[i];
    power = power * BASE;
  }
  // helper: match partial haystack
  // BE CAREFUL ! the matching function
  auto IsMatch = [&](int begin) {
    for (int i = 0; i < needle.length(); i++) {
      if (haystack[begin + i] != needle[i])
        return false;
    }
    return true;
  };
  // check first
  if (hashHaystack == hashNeedle && IsMatch(0)) {
    return 0;
  }
  // iterate
  for (int i = needle.length(); i < haystack.length(); i++) {
    // add next char first, and cut by power later
    hashHaystack = hashHaystack * BASE + haystack[i];
    hashHaystack -= power * haystack[i - needle.length()];
    // check later
    if (hashHaystack == hashNeedle && IsMatch(i - needle.length() + 1)) {
      return i - needle.length() + 1;
    }
  }
  // no last update: we checked first
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
