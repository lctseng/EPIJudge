#include "test_framework/generic_test.h"
#include <deque>
#include <vector>
using std::abs;
using std::deque;
using std::vector;

void Accumulate(deque<int> &sum, const vector<int> &num, int multiply,
                int base) {
  for (int i = 0; i < num.size(); i++) {
    sum[base + i] += num[i] * multiply;
  }
}

vector<int> Multiply(vector<int> num1, vector<int> num2) {
  bool isNeg = false;
  if (num1[0] * num2[0] < 0) {
    isNeg = true;
  }
  num1[0] = abs(num1[0]);
  num2[0] = abs(num2[0]);
  // TODO - you fill in here.
  deque<int> result(num1.size() + num2.size(), 0);
  int base = num2.size();
  for (int i = num2.size() - 1; i >= 0; i--) {
    Accumulate(result, num1, num2[i], base--);
  }
  // value fixing
  for (int i = result.size() - 1; i >= 0; i--) {
    if (result[i] >= 10) {
      int d = result[i] / 10;
      result[i] %= 10;
      if (i > 0) {
        result[i - 1] += d;
      } else {
        result.push_front(d);
      }
    }
  }
  // handling empty
  if (result.empty())
    result.push_front(0);
  // handling zeros
  while (result.size() > 1 && result.front() == 0)
    result.pop_front();
  if (isNeg) {
    result.front() *= -1;
  }
  return vector<int>(result.begin(), result.end());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num1", "num2"};
  return GenericTestMain(args, "int_as_array_multiply.cc",
                         "int_as_array_multiply.tsv", &Multiply,
                         DefaultComparator{}, param_names);
}
