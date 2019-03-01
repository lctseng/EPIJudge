#include "test_framework/generic_test.h"
#include <string>
using std::string;

bool CanFormPalindrome(const string &s) {
  // count every character
  int count[256] = {0};
  for (char c : s) {
    ++count[c];
  }
  int numOdd = 0;
  for (int i = 0; i < 256; i++) {
    if (count[i] & 1) {
      if (++numOdd > 1)
        return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_permutable_to_palindrome.cc",
                         "is_string_permutable_to_palindrome.tsv",
                         &CanFormPalindrome, DefaultComparator{}, param_names);
}
