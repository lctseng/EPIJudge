#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::swap;
using std::vector;

struct DuplicateAndMissing {
  int duplicate, missing;
};

DuplicateAndMissing FindDuplicateMissing(const vector<int> &A) {
  int v = 0;
  int n = A.size();
  for (int i = 0; i < n; i++) {
    v ^= i ^ A[i];
  }
  // v is now D xor M
  int lowbit = v & (-v);
  // group into 2. either miss or dup has that bit!
  int candidate = 0;
  for (int i = 0; i < n; i++) {
    if (i & lowbit) {
      candidate ^= i;
    }
    if (A[i] & lowbit) {
      candidate ^= A[i];
    }
  }
  // now, candidate may be miss or dup
  // search!
  for (int dup : A) {
    if (dup == candidate) {
      // exist in A, candidate is dup
      return {candidate, v ^ candidate};
    }
  }
  // candidate is miss
  return {v ^ candidate, candidate};
}
template <>
struct SerializationTraits<DuplicateAndMissing>
    : UserSerTraits<DuplicateAndMissing, int, int> {};

bool operator==(const DuplicateAndMissing &lhs,
                const DuplicateAndMissing &rhs) {
  return std::tie(lhs.duplicate, lhs.missing) ==
         std::tie(rhs.duplicate, rhs.missing);
}

std::ostream &operator<<(std::ostream &out, const DuplicateAndMissing &x) {
  return out << "duplicate: " << x.duplicate << ", missing: " << x.missing;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "search_for_missing_element.cc", "find_missing_and_duplicate.tsv",
      &FindDuplicateMissing, DefaultComparator{}, param_names);
}
