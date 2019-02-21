#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

string IntToString(int _x) {
  long long x = _x;
  string res;
  if (x < 0) {
    res.push_back('-');
    x *= -1;
  }
  int exp = 1;
  while (x / exp >= 10)
    exp *= 10;
  while (exp > 0) {
    int d = x / exp;
    x %= exp;
    exp /= 10;
    res.push_back(d + '0');
  }
  // fix the case for 0
  if (res.empty() || (res.length() == 1 && res[0] == '-')) {
    res.push_back('0');
  }
  return res;
}
int StringToInt(const string &s) {
  if (s.empty())
    return 0;
  // throws if meet non-digit character
  int sign = 1, index = 0;
  long long val = 0;
  if (s[0] == '-') {
    sign = -1;
    ++index;
  };
  for (; index < s.length(); index++) {
    char c = s[index];
    if (c >= '0' && c <= '9') {
      val = val * 10 + (c - '0');
    } else {
      throw "Encounter error characters";
    }
  }
  return val * sign;
}
void Wrapper(int x, const string &s) {
  if (IntToString(x) != s) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
