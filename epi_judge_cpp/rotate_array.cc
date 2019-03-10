#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <vector>
using std::reverse;
using std::size;
using std::vector;

void RotateArray(int rotate_amount, vector<int> *A_ptr) {
  vector<int> &A = *A_ptr;
  rotate_amount %= size(A);
  // use reverse
  int left = size(A) - rotate_amount;
  reverse(A.begin(), A.begin() + left);
  reverse(A.begin() + left, A.end());
  reverse(A.begin(), A.end());
}
vector<int> RotateArrayWrapper(TimedExecutor &executor, vector<int> A,
                               int rotate_amount) {
  executor.Run([&] { RotateArray(rotate_amount, &A); });
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A", "rotate_amount"};
  return GenericTestMain(args, "rotate_array.cc", "rotate_array.tsv",
                         &RotateArrayWrapper, DefaultComparator{}, param_names);
}
