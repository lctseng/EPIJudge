#include "test_framework/generic_test.h"
#include <cstring>
#include <string>
using std::string;

bool IsPalindrome(const string &s) {
  int left = 0, right = s.length() - 1;
  while (left < right) {
    // advance to valid char
    while (left < right && !isalnum(s[left]))
      left++;
    while (left < right && !isalnum(s[right]))
      right--;
    if (tolower(s[left++]) != tolower(s[right--]))
      return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_palindromic_punctuation.cc",
                         "is_string_palindromic_punctuation.tsv", &IsPalindrome,
                         DefaultComparator{}, param_names);
}
