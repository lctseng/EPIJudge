#include "test_framework/generic_test.h"
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using std::make_pair;
using std::multimap;
using std::string;
using std::unordered_map;
using std::vector;

// Finds the candidates which may occur > n / k times.
vector<string>
SearchFrequentItems(int k, vector<string>::const_iterator stream_begin,
                    const vector<string>::const_iterator stream_end) {
  // freqToCandidate: tree map int => string
  // CandidateToTreeIt: hash map string =>  iterator
  // this can find k most candidate
  multimap<int, string> freqToCandidate;
  unordered_map<string, multimap<int, string>::iterator> CandidateToTreeIt;
  auto currentIt = stream_begin;
  while (currentIt != stream_end) {
    auto &currentStr = *(currentIt++);
    // update freq
    auto candTreeit = CandidateToTreeIt.find(currentStr);
    if (candTreeit != CandidateToTreeIt.end()) {
      // old entry, increase freq
      auto oldTreeIt = candTreeit->second;
      int oldFreq = oldTreeIt->first;
      freqToCandidate.erase(oldTreeIt);
      candTreeit->second =
          freqToCandidate.insert(make_pair(oldFreq + 1, currentStr));
    } else {
      if (freqToCandidate.size() < k) {
        // enough space, directly add as new entry
        auto treeIt = freqToCandidate.insert(make_pair(1, currentStr));
        CandidateToTreeIt[currentStr] = treeIt;
      } else {
        // decrease the one with minimal freq
        // remove if reach 0
        auto minFreqIt = freqToCandidate.begin();
        int newFreq = minFreqIt->first - 1;
        auto candStr = minFreqIt->second;
        if (newFreq > 0) {
          // insert back
          freqToCandidate.erase(minFreqIt);
          auto newTreeIt = freqToCandidate.insert(make_pair(newFreq, candStr));
          CandidateToTreeIt[candStr] = newTreeIt;
        } else {
          // remove this entry from tree
          freqToCandidate.erase(minFreqIt);
          CandidateToTreeIt.erase(candStr);
        }
      }
    }
  }
  // in the second pass, only foucs on these element
  unordered_map<string, int> candidateFreq;
  for (auto &data : freqToCandidate) {
    candidateFreq[data.second] = 0;
  }
  // find their freq
  currentIt = stream_begin;
  while (currentIt != stream_end) {
    auto &currentStr = *(currentIt++);
    auto it = candidateFreq.find(currentStr);
    if (it != candidateFreq.end()) {
      it->second++;
    }
  }
  // finally return those > n/k times
  vector<string> res;
  int countReq = (stream_end - stream_begin) / k;
  for (auto &data : candidateFreq) {
    if (data.second > countReq) {
      res.push_back(data.first);
    }
  }
  return res;
}
vector<string> SearchFrequentItemsWrapper(int k, vector<string> &stream) {
  return SearchFrequentItems(k, begin(stream), end(stream));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "stream"};
  return GenericTestMain(
      args, "search_frequent_items.cc", "search_frequent_items.tsv",
      &SearchFrequentItemsWrapper,
      &UnorderedComparator<std::vector<std::string>>, param_names);
}
