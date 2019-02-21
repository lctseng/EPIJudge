#include "test_framework/generic_test.h"
#include <string>
using std::string;

inline int charToValue(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  else
    return 0;
}

inline char valueToChar(int value) {
  if (value < 10)
    return value + '0';
  else
    return 'A' + (value - 10);
}

string ConvertBase(const string &num_as_string, int b1, int b2) {
  // convert to integer
  long long value = 0;
  int index = 0;
  bool isNeg = false;
  if (num_as_string[0] == '-') {
    isNeg = true;
    ++index;
  }
  for (; index < num_as_string.length(); ++index) {
    value = value * b1 + charToValue(num_as_string[index]);
  }
  string res;
  while (value > 0) {
    res.push_back(valueToChar(value % b2));
    value /= b2;
  }
  if (res.empty())
    res.push_back('0');
  if (isNeg && res.back() != 0) {
    res.append(1, '-');
  }
  return string(rbegin(res), rend(res));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
