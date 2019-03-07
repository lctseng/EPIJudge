#include "test_framework/generic_test.h"

#define BIT_SIZE (sizeof(unsigned long long) * 8)

unsigned long long Add(unsigned long long x, unsigned long long y) {
  unsigned long long res = 0;
  unsigned long long carryin = 0;
  int mask = 1;
  unsigned long long ored = x | y;
  while (ored) {
    auto xk = (x & mask);
    auto yk = (y & mask);
    res |= xk ^ yk ^ carryin;
    // next carry in
    carryin = ((xk & yk) | (carryin & yk) | (xk & carryin)) << 1;
    // cancel this bit
    mask <<= 1;
    ored >>= 1;
  }
  return res | carryin;
}

unsigned long long Multiply(unsigned long long x, unsigned long long y) {
  // like elementrary school multiplication
  unsigned long long res = 0;
  while (y) {
    if (y & 1) {
      res = Add(res, x);
    }
    y >>= 1;
    x <<= 1;
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "primitive_multiply.cc",
                         "primitive_multiply.tsv", &Multiply,
                         DefaultComparator{}, param_names);
}
