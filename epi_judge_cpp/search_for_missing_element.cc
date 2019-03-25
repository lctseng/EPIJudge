#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::swap;
using std::vector;

struct DuplicateAndMissing {
  int duplicate, missing;
};

DuplicateAndMissing FindDuplicateMissing(const vector<int> &A) {
  int n = A.size();
  // numbers are from 0 to n-1
  // compute xorA: all other value xored except D, M
  // xored with 0 to n-1, we got D ^ M
  int DxorM = 0;
  for (int val : A) {
    DxorM ^= val;
  }
  for (int val = 0; val < n; val++) {
    DxorM ^= val;
  }
  // for one bit BIT: x&(-x): that is D and M differs
  int lowbit = DxorM & (-DxorM);
  // for all A with BIT, xored them
  int candidate = 0;
  for (int val : A) {
    if (val & lowbit)
      candidate ^= val;
  }
  // for 0 to n-1 with BIT, xored them
  for (int val = 0; val < n; val++) {
    if (val & lowbit)
      candidate ^= val;
  }
  // the final result is either D or M
  // search this value from A, if found, it is D
  for (int val : A) {
    if (val == candidate) {
      return {candidate, DxorM ^ candidate};
    }
  }
  // end of search, it is M
  return {DxorM ^ candidate, candidate};
}

// DuplicateAndMissing FindDuplicateMissing(const vector<int> &A) {
//   int v = 0;
//   int n = A.size();
//   for (int i = 0; i < n; i++) {
//     v ^= i ^ A[i];
//   }
//   // v is now D xor M
//   int lowbit = v & (-v);
//   // group into 2. either miss or dup has that bit!
//   int candidate = 0;
//   for (int i = 0; i < n; i++) {
//     if (i & lowbit) {
//       candidate ^= i;
//     }
//     if (A[i] & lowbit) {
//       candidate ^= A[i];
//     }
//   }
//   // now, candidate may be miss or dup
//   // search!
//   for (int dup : A) {
//     if (dup == candidate) {
//       // exist in A, candidate is dup
//       return {candidate, v ^ candidate};
//     }
//   }
//   // candidate is miss
//   return {v ^ candidate, candidate};
// }

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
