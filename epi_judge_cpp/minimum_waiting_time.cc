#include "test_framework/generic_test.h"
#include <vector>
using std::sort;
using std::vector;

int MinimumTotalWaitingTime(vector<int> service_times) {
  sort(service_times.begin(), service_times.end());
  int totalSum = 0;
  int currentSum = 0;
  for (int currentTask : service_times) {
    totalSum += currentSum;
    currentSum += currentTask;
  }
  return totalSum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"service_times"};
  return GenericTestMain(args, "minimum_waiting_time.cc",
                         "minimum_waiting_time.tsv", &MinimumTotalWaitingTime,
                         DefaultComparator{}, param_names);
}
