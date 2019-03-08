#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>
using std::abs;
using std::bind;
using std::lower_bound;
using std::unordered_map;
using std::vector;

const double kRndMax = (double)RAND_MAX - 1;

double UniformZeroOne() { return (double)(rand()) / kRndMax; }

int NonuniformRandomNumberGeneration(const vector<int> &values,
                                     const vector<double> &probabilities) {
  double r = UniformZeroOne();
  double sum = 0;
  for (int i = 0; i < probabilities.size(); i++) {
    sum += probabilities[i];
    if (sum >= r)
      return values[i];
  }
  return values.back();
}

int NonuniformRandomNumberGenerationSlow(const vector<int> &values,
                                         const vector<double> &probabilities) {
  double r = UniformZeroOne();
  vector<double> prefixSum(1, probabilities.front());
  for (int i = 1; i < probabilities.size(); i++) {
    prefixSum.push_back(prefixSum.back() + probabilities[i]);
  }
  prefixSum.back() = 1.0; // fix potential precision error
  int index =
      lower_bound(prefixSum.begin(), prefixSum.end(), r) - prefixSum.begin();
  return values[index];
}
bool NonuniformRandomNumberGenerationRunner(
    TimedExecutor &executor, const vector<int> &values,
    const vector<double> &probabilities) {
  constexpr int kN = 1000000;
  vector<int> results;

  executor.Run([&] {
    for (int i = 0; i < kN; ++i) {
      results.emplace_back(
          NonuniformRandomNumberGeneration(values, probabilities));
    }
  });

  unordered_map<int, int> counts;
  for (int result : results) {
    ++counts[result];
  }
  for (int i = 0; i < values.size(); ++i) {
    const int v = values[i];
    const double p = probabilities[i];
    if (kN * p < 50 || kN * (1.0 - p) < 50) {
      continue;
    }
    const double sigma = sqrt(kN * p * (1.0 - p));
    if (abs(counts[v] - (p * kN)) > 5 * sigma) {
      return false;
    }
  }
  return true;
}

void NonuniformRandomNumberGenerationWrapper(
    TimedExecutor &executor, const vector<int> &values,
    const vector<double> &probabilities) {
  RunFuncWithRetries(bind(NonuniformRandomNumberGenerationRunner,
                          std::ref(executor), std::cref(values),
                          std::cref(probabilities)));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "values", "probabilities"};
  return GenericTestMain(args, "nonuniform_random_number.cc",
                         "nonuniform_random_number.tsv",
                         &NonuniformRandomNumberGenerationWrapper,
                         DefaultComparator{}, param_names);
}
