#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <set>
#include <vector>
using std::swap;
using std::vector;

void EvenOdd(vector<int> *A_ptr) {
  int left = 0, right = A_ptr->size() - 1;
  // left: next even pos
  // right: next odd pos
  while (left <= right) {
    if (A_ptr->at(left) & 1) {
      // found odd in even
      swap(A_ptr->at(left), A_ptr->at(right--));
    } else {
      ++left;
    }
  }
}
void EvenOddWrapper(TimedExecutor &executor, vector<int> A) {
  std::multiset<int> before(begin(A), end(A));

  executor.Run([&] { EvenOdd(&A); });

  bool in_odd = false;
  for (int a : A) {
    if (a % 2 == 0) {
      if (in_odd) {
        throw TestFailure("Even elements appear in odd part");
      }
    } else {
      in_odd = true;
    }
  }

  std::multiset<int> after(begin(A), end(A));
  if (before != after) {
    throw TestFailure("Elements mismatch");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A"};
  return GenericTestMain(args, "even_odd_array.cc", "even_odd_array.tsv",
                         &EvenOddWrapper, DefaultComparator{}, param_names);
}
