#include "test_framework/generic_test.h"
#include <unordered_set>
#include <vector>
using std::unordered_set;
using std::vector;

bool TestCollatzConjecture(int n) {
  // TODO - you fill in here.
  // set current = i, where i = 1 to n
  // while current >= i
  // if odd, triple and +1, add to hash as true
  // if even, halves it
  // if < i after halfing, go next number
  //   if not, add to hash as true
  unordered_set<int> valid;
  for (int i = 3; i <= n; i += 2) {
    int current = i;
    unordered_set<int> seen;
    while (current >= i) {
      if (seen.count(current))
        return false; // not hold! we go into loop
      seen.insert(current);
      if (current & 1) {
        // BE CAUREFUL: formally we need to check overflow
        if (valid.count(current)) // we have existing result, this is valid
          break;
        valid.insert(current);
        current = current * 3 + 1;
      } else {
        current >>= 1;
      }
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "collatz_checker.cc", "collatz_checker.tsv",
                         &TestCollatzConjecture, DefaultComparator{},
                         param_names);
}
