#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <vector>
using std::bind;
using std::iota;
using std::swap;
using std::unordered_map;
using std::vector;

// Returns a random k-sized subset of {0, 1, ..., n - 1}.
vector<int> RandomSubset(int n, int k) {
  unordered_map<int, int> res;

  for (int i = 0; i < k; i++) {
    int remainSz = n - i;
    int targetIndex = rand() % remainSz + i;
    // swap A[i] and A[targetIndex]
    auto it1 = res.find(i);
    auto it2 = res.find(targetIndex);
    if (it1 != res.end() && it2 != res.end()) {
      swap(it1->second, it2->second);
    } else if (it1 != res.end() && it2 == res.end()) {
      res[targetIndex] = it1->second;
      it1->second = targetIndex;
    } else if (it1 == res.end() && it2 != res.end()) {
      res[i] = it2->second;
      it2->second = i;
    } else {
      res[i] = targetIndex;
      res[targetIndex] = i;
    }
  }
  vector<int> ans;
  for (int i = 0; i < k; i++) {
    ans.push_back(res[i]);
  }
  return ans;
}
bool RandomSubsetRunner(TimedExecutor &executor, int n, int k) {
  vector<vector<int>> results;

  executor.Run([&] {
    std::generate_n(back_inserter(results), 100000,
                    std::bind(RandomSubset, n, k));
  });

  int total_possible_outcomes = BinomialCoefficient(n, k);
  vector<int> A(n);
  iota(begin(A), end(A), 0);
  vector<vector<int>> combinations;
  for (int i = 0; i < BinomialCoefficient(n, k); ++i) {
    combinations.emplace_back(ComputeCombinationIdx(A, n, k, i));
  }
  vector<int> sequence;
  for (vector<int> result : results) {
    sort(begin(result), end(result));
    sequence.emplace_back(
        distance(begin(combinations),
                 find(begin(combinations), end(combinations), result)));
  }
  return CheckSequenceIsUniformlyRandom(sequence, total_possible_outcomes,
                                        0.01);
}

void RandomSubsetWrapper(TimedExecutor &executor, int n, int k) {
  RunFuncWithRetries(bind(RandomSubsetRunner, std::ref(executor), n, k));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "n", "k"};
  return GenericTestMain(args, "random_subset.cc", "random_subset.tsv",
                         &RandomSubsetWrapper, DefaultComparator{},
                         param_names);
}
