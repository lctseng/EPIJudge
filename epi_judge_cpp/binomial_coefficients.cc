#include "test_framework/generic_test.h"
using std::swap;

int GCD(int a, int b) {
  if (a < b)
    swap(a, b);
  while (b > 0) {
    int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}
int ComputeBinomialCoefficient(int n, int k) {
  int result = 1;
  int upperBegin = n - k;
  for (int lower = 1; lower <= k; lower++) {
    // may overflow at this mutiply
    int toMul = upperBegin + lower;
    if (toMul % lower == 0) {
      toMul /= lower;
      result *= toMul;
    } else if (result % lower == 0) {
      result /= lower;
      result *= toMul;
    } else {
      int lowerCopy = lower;
      int gcdRL = GCD(result, lowerCopy);
      result /= gcdRL;
      lowerCopy /= gcdRL;
      int gcdHL = GCD(toMul, lowerCopy);
      toMul /= gcdHL;
      lowerCopy /= gcdHL;
      result *= toMul;
      result /= lowerCopy;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "binomial_coefficients.cc", "binomial_coefficients.tsv",
      &ComputeBinomialCoefficient, DefaultComparator{}, param_names);
}
