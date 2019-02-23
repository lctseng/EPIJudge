#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <array>
#include <stack>
#include <string>
#include <vector>
using std::array;
using std::stack;
using std::vector;
const int kNumPegs = 3;

void ComputeTowerHanoi(vector<vector<int>> &ops, int num_rings, int from,
                       int to) {
  if (num_rings <= 0)
    return;
  // we first move n - 1 rings from "from" to intermediate
  int intermediate;
  switch (from) {
  case 0:
    intermediate = to == 1 ? 2 : 1;
    break;
  case 1:
    intermediate = to == 0 ? 2 : 0;
    break;
  case 2:
    intermediate = to == 0 ? 1 : 0;
    break;
  }
  ComputeTowerHanoi(ops, num_rings - 1, from, intermediate);
  // then move last ring from to to
  ops.push_back({from, to});
  // move remaining from intermidate to to
  ComputeTowerHanoi(ops, num_rings - 1, intermediate, to);
}

vector<vector<int>> ComputeTowerHanoi(int num_rings) {
  vector<vector<int>> ops;
  ComputeTowerHanoi(ops, num_rings, 0, 1);
  return ops;
}
void ComputeTowerHanoiWrapper(TimedExecutor &executor, int num_rings) {
  array<stack<int>, kNumPegs> pegs;
  for (int i = num_rings; i >= 1; --i) {
    pegs[0].push(i);
  }

  vector<vector<int>> result =
      executor.Run([&] { return ComputeTowerHanoi(num_rings); });

  for (const vector<int> &operation : result) {
    int from_peg = operation[0], to_peg = operation[1];
    if (!pegs[to_peg].empty() && pegs[from_peg].top() >= pegs[to_peg].top()) {
      throw TestFailure("Illegal move from " +
                        std::to_string(pegs[from_peg].top()) + " to " +
                        std::to_string(pegs[to_peg].top()));
    }
    pegs[to_peg].push(pegs[from_peg].top());
    pegs[from_peg].pop();
  }
  array<stack<int>, kNumPegs> expected_pegs1, expected_pegs2;
  for (int i = num_rings; i >= 1; --i) {
    expected_pegs1[1].push(i);
  }
  for (int i = num_rings; i >= 1; --i) {
    expected_pegs2[2].push(i);
  }
  if (pegs != expected_pegs1 && pegs != expected_pegs2) {
    throw TestFailure("Pegs doesn't place in the right configuration");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "num_rings"};
  return GenericTestMain(args, "hanoi.cc", "hanoi.tsv",
                         &ComputeTowerHanoiWrapper, DefaultComparator{},
                         param_names);
}
