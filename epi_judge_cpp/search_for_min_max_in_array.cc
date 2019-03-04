#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::vector;
struct MinMax {
  int smallest, largest;
};

MinMax FindMinMax(const vector<int> &A) {
  int small = A[0], large = A[0];
  int sz = A.size();
  int i = 0;
  if (sz & 1) {
    i++;
  }
  for (; i < sz; i += 2) {
    if (A[i] < A[i + 1]) {
      if (A[i + 1] > large)
        large = A[i + 1];
      if (A[i] < small)
        small = A[i];
    } else {
      if (A[i] > large)
        large = A[i];
      if (A[i + 1] < small)
        small = A[i + 1];
    }
  }
  return {small, large};
}
template <>
struct SerializationTraits<MinMax> : UserSerTraits<MinMax, int, int> {};

bool operator==(const MinMax &lhs, const MinMax &rhs) {
  return std::tie(lhs.smallest, lhs.largest) ==
         std::tie(rhs.smallest, rhs.largest);
}

std::ostream &operator<<(std::ostream &out, const MinMax &x) {
  return out << "min: " << x.smallest << ", max: " << x.largest;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "search_for_min_max_in_array.cc",
                         "search_for_min_max_in_array.tsv", &FindMinMax,
                         DefaultComparator{}, param_names);
}
