#include "test_framework/generic_test.h"
#include <string>
#include <unordered_map>
using std::size;
using std::string;
using std::unordered_map;

typedef unordered_map<uint64_t, bool> Cache;

// assume it is a valid regex
// i for regex
// j for string, must including substr
bool IsMatchHelper(Cache &cache, const string &regex, const string &s, int i,
                   int j, bool allowSubstr) {
  // BE CAREFUL! substring match is allowed
  if (i == size(regex)) {
    // empty regex always match anything
    return true;
  }
  // check this char of regex
  uint64_t cacheKey =
      (uint64_t)i << 31 | (uint64_t)j << 1 | (uint64_t)allowSubstr;
  auto it = cache.find(cacheKey);
  if (it != cache.end())
    return it->second;
  char c = regex[i];
  if (c == '^') {
    // start anchor
    return cache[cacheKey] =
               j == 0 && IsMatchHelper(cache, regex, s, i + 1, j, false);
  } else if (c == '$') {
    // end anchor
    return j == size(s);
  } else {

    // beware that s maybe empty...
    // alnum or dot. Note that we will NOT let 'star' appear in the current char

    bool hasStar = i < size(regex) - 1 && regex[i + 1] == '*';
    bool thisMatch = j < size(s) && (regex[i] == '.' || regex[i] == s[j]);

    // BE CAREFUL: since we dont need exactly match
    // we can start form any substring
    if (hasStar) {
      // wildcard activate or not activate
      return cache[cacheKey] =
                 (thisMatch && IsMatchHelper(cache, regex, s, i, j + 1,
                                             false)) || // match current
                 (allowSubstr && j < size(s) &&
                  IsMatchHelper(cache, regex, s, i, j + 1,
                                allowSubstr)) || // match substr
                 IsMatchHelper(cache, regex, s, i + 2, j,
                               false); // skip current
    } else {
      return cache[cacheKey] =
                 (thisMatch && IsMatchHelper(cache, regex, s, i + 1, j + 1,
                                             false)) || // match current
                 (allowSubstr && j < size(s) &&
                  IsMatchHelper(cache, regex, s, i, j + 1,
                                allowSubstr)); // match substr
    }
  }
}

bool IsMatch(const string &regex, const string &s) {
  Cache cache;
  return IsMatchHelper(cache, regex, s, 0, 0, true);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"regex", "s"};
  return GenericTestMain(args, "regular_expression.cc",
                         "regular_expression.tsv", &IsMatch,
                         DefaultComparator{}, param_names);
}
