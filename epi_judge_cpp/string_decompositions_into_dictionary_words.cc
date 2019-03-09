#include "test_framework/generic_test.h"
#include <string>
#include <unordered_map>
#include <vector>
using std::sort;
using std::string;
using std::unordered_map;
using std::vector;

vector<int> FindAllSubstrings(const string &s, const vector<string> &words) {
  if (words.empty())
    return {};
  int wordLen = words[0].length();
  vector<int> res;
  // try every offset from 0 to wordLen - 1
  unordered_map<string, int> _wordCount;
  int _remainCount = words.size();
  for (auto &word : words) {
    _wordCount[word]++;
  }
  for (int sOffset = 0; sOffset < wordLen; sOffset++) {
    // use a map/remainCount to maintain coverage
    unordered_map<string, int> wordCount = _wordCount;
    int remainCount = _remainCount;
    // for each offset, jump wordLen to tokenize
    // j: pass-over-1 the last in-map index, advance and update
    int i = sOffset, j = sOffset;
    // BE CAREFUL! wordLen > s.length cause overflow!
    int maxJ = (int)s.length() - wordLen;
    while (j <= maxJ) {
      // remainCount > 0, advance j
      // state for leave loop
      int state = 0; // 0: length exceeds, 1: unknown word, 2: exceeds
      while (j <= maxJ) {
        string currentWord = s.substr(j, wordLen);
        auto it = wordCount.find(currentWord);
        if (it != wordCount.end()) {
          it->second--;
          if (it->second >= 0) {
            remainCount--;
          } else if (it->second < 0) {
            // found extra word
            state = 2;
            break;
          }
          // continue next loop
          j += wordLen;
        } else {
          // found an unknown word
          state = 1;
          break;
        }
      }
      if (state == 0) {
        // BE CAREFUL!
        // for out of range but meet condition, we have last update
        // to avoid double counted
        break;
      }
      if (remainCount == 0) {
        // covered!
        // compute the starting index by j - wordLen * words.size()
        res.push_back(j - wordLen * words.size());
      }
      // meet a unknown word: reset to empty, i = j = j += wordLen
      if (state == 1) {
        i = j = j + wordLen;
        wordCount = _wordCount;
        remainCount = _remainCount;
      } else {
        // if countMap < 0, means too many used words, shrink left
        // advance i and update map, until we found same as j,
        string extraWord = s.substr(j, wordLen);
        while (i < j) {
          string currentWord = s.substr(i, wordLen);
          if (currentWord == extraWord) {
            ++wordCount[currentWord]; // balance the count
            i += wordLen;             // i discard that word
            break;
          } else {
            // advance it
            ++wordCount[currentWord];
            ++remainCount;
            i += wordLen;
          }
        }
        // BE CAREFUL!
        // always update j
        j += wordLen; // j pickup that word
      }
    }
    // BE CAREFUL: last update
    // last update
    if (remainCount == 0) {
      res.push_back(j - wordLen * words.size());
    }
  }
  sort(res.begin(), res.end());
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s", "words"};
  return GenericTestMain(args, "string_decompositions_into_dictionary_words.cc",
                         "string_decompositions_into_dictionary_words.tsv",
                         &FindAllSubstrings, DefaultComparator{}, param_names);
}
