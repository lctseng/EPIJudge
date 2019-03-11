#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::swap;
using std::vector;

struct BeginEnd {
  int begin, end;
};

BeginEnd SearchAbsSorted(const vector<int> &A, int n) {
  if (A.size() < 2)
    return {-1, -1};
  // handle special case: when A contains 0 and n is extact one of them
  if (A[0] == 0) {
    // search for existance
    for (int i = 1; i < A.size(); i++) {
      if (A[i] == n) {
        return {0, i};
      }
    }
  }
  // two pointer,
  // A is not made from zero
  // small and large will ignore zeros
  int small = A.size(), large = A.size();
  int smallDir = -1, largeDir = -1;
  auto AdvanceSmall = [&]() {
    while (true) {
      // keep going
      small += smallDir;
      if (small < 0) {
        small = 1;
        smallDir = 1;
      }
      if (small >= A.size() ||
          ((smallDir < 0 && A[small] < 0) || (smallDir > 0 && A[small] > 0))) {
        break;
      }
    }
  };
  auto AdvanceLarge = [&]() {
    while (true) {
      // keep going
      large += largeDir;
      if (large < 0) {
        large = 1;
        largeDir = 1;
      }
      if (large >= A.size() ||
          ((largeDir < 0 && A[large] > 0) || (largeDir > 0 && A[large] < 0))) {
        break;
      }
    }
  };
  AdvanceLarge();
  AdvanceSmall();
  auto IsMeet = [&]() { return small == large; };
  while (!IsMeet()) {
    int current = A[small] + A[large];
    if (current > n) {
      // too large
      AdvanceLarge();
    } else if (current < n) {
      // too small
      AdvanceSmall();
    } else if (current == n) {
      if (small > large) {
        swap(small, large);
      }
      return {small, large};
    }
  }
  return {-1, -1};
}

template <>
struct SerializationTraits<BeginEnd> : UserSerTraits<BeginEnd, int, int> {};

bool operator==(const BeginEnd &lhs, const BeginEnd &rhs) {
  return std::tie(lhs.begin, lhs.end) == std::tie(rhs.begin, rhs.end);
}

std::ostream &operator<<(std::ostream &out, const BeginEnd &x) {
  return out << "begin: " << x.begin << ", end: " << x.end;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "n"};
  return GenericTestMain(args, "abs_sorted.cc", "abs_sorted.tsv",
                         &SearchAbsSorted, DefaultComparator{}, param_names);
}
