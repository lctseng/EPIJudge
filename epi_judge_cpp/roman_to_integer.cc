#include "test_framework/generic_test.h"
#include <string>
using std::string;
int letterToNum(char c) {
  switch (c) {
  case 'I':
    return 1;
  case 'V':
    return 5;
  case 'X':
    return 10;
  case 'L':
    return 50;
  case 'C':
    return 100;
  case 'D':
    return 500;
  case 'M':
    return 1000;
  default:
    return 0;
  }
}

int RomanToInteger(const string &s) {
  int sum = 0;
  int len = s.length();
  for (int i = 0; i < len; i++) {
    char curChar = s[i];
    char nextChar = i < len - 1 ? s[i + 1] : 0;
    if ((curChar == 'I' && (nextChar == 'V' || nextChar == 'X')) ||
        (curChar == 'X' && (nextChar == 'L' || nextChar == 'C')) ||
        (curChar == 'C' && (nextChar == 'D' || nextChar == 'M'))) {
      sum += (letterToNum(nextChar) - letterToNum(curChar));
      i++;
    } else {
      // just accumulate
      sum += letterToNum(curChar);
    }
  }

  return sum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "roman_to_integer.cc", "roman_to_integer.tsv",
                         &RomanToInteger, DefaultComparator{}, param_names);
}
