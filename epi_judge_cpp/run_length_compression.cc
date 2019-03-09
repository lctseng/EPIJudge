#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include <cstring>
#include <string>
using std::string;
using std::to_string;
string Decoding(const string &s) {
  int i = 0;
  string res;
  while (i < s.length()) {
    // read length
    int len = 0;
    while (i < s.length() && isdigit(s[i])) {
      len = len * 10 + (s[i] - '0');
      ++i;
    }
    // read char
    char c = s[i++];
    // i points to next number
    res.append(len, c);
  }
  return res;
}
string Encoding(const string &s) {
  string res;
  char current = 0;
  int count = 0;
  for (int i = 0; i <= s.length(); i++) {
    if (i < s.length() && s[i] == current) {
      count++;
    } else {
      // add if needed
      if (count) {
        res.append(to_string(count));
        res.push_back(current);
      }
      // set new
      if (i < s.length()) {
        count = 1;
        current = s[i];
      }
    }
  }
  return res;
}
void RleTester(const string &encoded, const string &decoded) {
  if (Decoding(encoded) != decoded) {
    throw TestFailure("Decoding failed");
  }
  if (Encoding(decoded) != encoded) {
    throw TestFailure("Encoding failed");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"encoded", "decoded"};
  return GenericTestMain(args, "run_length_compression.cc",
                         "run_length_compression.tsv", &RleTester,
                         DefaultComparator{}, param_names);
}
