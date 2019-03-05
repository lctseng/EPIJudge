#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <string>
#include <unordered_set>
#include <vector>
using std::size;
using std::string;
using std::unordered_set;
using std::vector;

vector<int> dp;
int maxWordLen, minWordLen;
// dp[i][j]: start from domain[i:], we can cut at domain[j:]
// so that current string is domain[i:j)

// can we cut domain[i:] successfully?
// if can, record all cut to dp before return
bool DecomposeIntoDictionaryWordsHelper(const string &domain,
                                        const unordered_set<string> &dictionary,
                                        int index) {
  if (index >= size(domain))
    return true;
  //  else if(size(domain) - index)
  else if (dp[index] >= 0) // computed
    return dp[index] > index;
  // compute manually and update dp
  string currentWord;
  for (int i = index; i < size(domain); i++) {
    currentWord.append(1, domain[i]);
    if (size(currentWord) < minWordLen)
      continue;
    else if (size(currentWord) > maxWordLen)
      break;
    // check ?
    if (dictionary.count(currentWord)) {
      if (DecomposeIntoDictionaryWordsHelper(domain, dictionary, i + 1)) {
        // can cut at here
        dp[index] = i + 1;
        return true;
      }
    }
  }
  // cannot find a cut here
  dp[index] = index;
  return false;
}

vector<string>
DecomposeIntoDictionaryWords(const string &domain,
                             const unordered_set<string> &dictionary) {
  if (dictionary.empty())
    return {};
  dp.assign(size(domain), -1);
  minWordLen = maxWordLen = dictionary.begin()->length();
  for (auto &word : dictionary) {
    auto cur = size(word);
    if (cur > maxWordLen)
      maxWordLen = cur;
    else if (cur < minWordLen) {
      minWordLen = cur;
    }
  }
  DecomposeIntoDictionaryWordsHelper(domain, dictionary, 0);
  // re-construct answer
  vector<string> res;
  if (dp[0] > 0) {
    int i = 0;
    int j = dp[i];
    while (i < size(domain)) {
      res.push_back(domain.substr(i, j - i));
      i = j;
      j = dp[j];
    }
  }
  return res;
}
void DecomposeIntoDictionaryWordsWrapper(
    TimedExecutor &executor, const string &domain,
    const unordered_set<string> &dictionary, bool decomposable) {
  vector<string> result = executor.Run(
      [&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
  if (!decomposable) {
    if (!result.empty()) {
      throw TestFailure("domain is not decomposable");
    }
    return;
  }

  if (std::any_of(std::begin(result), std::end(result),
                  [&](const std::string &s) { return !dictionary.count(s); })) {
    throw TestFailure("Result uses words not in dictionary");
  }

  if (std::accumulate(std::begin(result), std::end(result), string()) !=
      domain) {
    throw TestFailure("Result is not composed into domain");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "domain", "dictionary",
                                       "decomposable"};
  return GenericTestMain(args, "is_string_decomposable_into_words.cc",
                         "is_string_decomposable_into_words.tsv",
                         &DecomposeIntoDictionaryWordsWrapper,
                         DefaultComparator{}, param_names);
}
