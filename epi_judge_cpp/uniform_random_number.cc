#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
#include <functional>
#include <random>
#include <vector>
using std::bind;
using std::default_random_engine;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;
int ZeroOneRandom() {
  default_random_engine gen((random_device())());
  uniform_int_distribution<int> dis(0, 1);
  return dis(gen);
}

int RandomNumWithBits(int bitControl) {
  int res = 0;
  while (bitControl) {
    res = (res << 1) | ZeroOneRandom();
    bitControl >>= 1;
  }
  return res;
}

int UniformRandom(int lower_bound, int upper_bound) {
  int range = upper_bound - lower_bound;
  // decompose range into 1,3,7,15,31,63,... etc
  vector<int> powers;
  int maxExp = 2;
  while (maxExp - 1 < range) {
    maxExp <<= 1;
  }
  int result = 0;
  --maxExp;
  while (true) {
    int r = RandomNumWithBits(maxExp);
    if (r <= range) {
      return r + lower_bound;
    }
  }
}
bool UniformRandomRunner(TimedExecutor &executor, int lower_bound,
                         int upper_bound) {
  vector<int> result;
  executor.Run([&] {
    std::generate_n(back_inserter(result), 100000,
                    std::bind(UniformRandom, lower_bound, upper_bound));
  });

  vector<int> sequence;
  std::transform(begin(result), end(result), back_inserter(sequence),
                 [lower_bound](int result) { return result - lower_bound; });
  return CheckSequenceIsUniformlyRandom(result, upper_bound - lower_bound + 1,
                                        0.01);
}

void UniformRandomWrapper(TimedExecutor &executor, int lower_bound,
                          int upper_bound) {
  RunFuncWithRetries(
      bind(UniformRandomRunner, std::ref(executor), lower_bound, upper_bound));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "lower_bound",
                                       "upper_bound"};
  return GenericTestMain(args, "uniform_random_number.cc",
                         "uniform_random_number.tsv", &UniformRandomWrapper,
                         DefaultComparator{}, param_names);
}
