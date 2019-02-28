#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
using std::bind;
using std::swap;
using std::vector;
void RandomSampling(int k, vector<int> *A_ptr) {
  int size = A_ptr->size();
  for (int i = 0; i < k; i++) {
    int remainCount = size - i;
    int sampleOffset = rand() % remainCount;
    swap(A_ptr->at(i), A_ptr->at(i + sampleOffset));
  }
}
bool RandomSamplingRunner(TimedExecutor &executor, int k, vector<int> A) {
  vector<vector<int>> results;

  executor.Run([&] {
    for (int i = 0; i < 100000; ++i) {
      RandomSampling(k, &A);
      results.emplace_back(begin(A), begin(A) + k);
    }
  });

  int total_possible_outcomes = BinomialCoefficient(A.size(), k);
  sort(begin(A), end(A));
  vector<vector<int>> combinations;
  for (int i = 0; i < BinomialCoefficient(A.size(), k); ++i) {
    combinations.emplace_back(ComputeCombinationIdx(A, A.size(), k, i));
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

void RandomSamplingWrapper(TimedExecutor &executor, int k,
                           const vector<int> &A) {
  RunFuncWithRetries(
      bind(RandomSamplingRunner, std::ref(executor), k, std::cref(A)));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "A"};
  return GenericTestMain(args, "offline_sampling.cc", "offline_sampling.tsv",
                         &RandomSamplingWrapper, DefaultComparator{},
                         param_names);
}
