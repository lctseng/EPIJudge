#include "test_framework/generic_test.h"
int Divide(int x, int y) {
  int quo = 0;
  // move y to same MSB as X
  // find msb of X
  int x2 = x;
  int msbOffsetX = 0;
  while (x2) {
    ++msbOffsetX;
    x2 >>= 1;
  }
  // find msb of Y
  int y2 = y;
  int msbOffsetY = 0;
  while (y2) {
    ++msbOffsetY;
    y2 >>= 1;
  }
  if (msbOffsetY <= msbOffsetX) {
    int diff = msbOffsetX - msbOffsetY;
    y <<= diff;
    int partialQuo = 1 << diff;
    while (partialQuo > 0) {
      if (x >= y) {
        quo += partialQuo;
        // BE CAREFUL
        // remenber to decrease number
        x -= y;
      }
      partialQuo >>= 1;
      y >>= 1;
    }
    return quo;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "primitive_divide.cc", "primitive_divide.tsv",
                         &Divide, DefaultComparator{}, param_names);
}
