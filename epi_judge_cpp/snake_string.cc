#include "test_framework/generic_test.h"
#include <string>
using std::string;

string SnakeString(const string &s) {
  // 3 round
  string res;
  for (int t = 0; t < 3; t++) {
    // round 0: take 1st
    // round 1: take 0th and 2nd
    // round 2: take 3rd
    for (int gStart = 0; gStart < s.length(); gStart += 4) {
      if (t == 0) {
        if (gStart + 1 < s.length())
          res.append(1, s[gStart + 1]);
      } else if (t == 1) {
        res.append(1, s[gStart]);
        if (gStart + 2 < s.length())
          res.append(1, s[gStart + 2]);
      } else {
        if (gStart + 3 < s.length())
          res.append(1, s[gStart + 3]);
      }
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "snake_string.cc", "snake_string.tsv",
                         &SnakeString, DefaultComparator{}, param_names);
}
