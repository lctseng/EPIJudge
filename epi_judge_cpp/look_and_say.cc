#include "test_framework/generic_test.h"
#include <string>
using std::move;
using std::string;
using std::to_string;

string LookAndSay(int n) {
  if (n == 0)
    return "";
  else if (n == 1)
    return "1";
  string prev = "1";
  // generate next
  for (int i = 1; i < n; i++) {
    string next;
    // generate next from prev
    int j = 0;
    while (j < prev.length()) {
      int endJ = j + 1;
      // find endJ(pass over 1)
      while (endJ < prev.length() && prev[endJ - 1] == prev[endJ])
        endJ++;
      int count = endJ - j;
      next.append(to_string(count));
      next.append(1, prev[j]);
      j = endJ;
    }
    // swap
    prev = move(next);
  }
  return prev;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "look_and_say.cc", "look_and_say.tsv",
                         &LookAndSay, DefaultComparator{}, param_names);
}
