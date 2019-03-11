#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <unordered_set>
#include <vector>
using std::get;
using std::sort;
using std::unordered_set;
using std::vector;

typedef unordered_set<uint64_t> Cache;

struct Jug {
  int low, high;
};

Cache cache;

bool CheckFeasibleHelper(const vector<Jug> &jugs, int L, int H) {
  if (L > H || (L < 0 && H < 0))
    return false;
  uint64_t cacheKey = (uint64_t)(L) << 32 | (uint64_t)H;
  if (cache.count(cacheKey))
    return false;
  // try use a jug
  for (auto &jug : jugs) {
    if ((jug.low >= L && jug.high <= H) || // base case: this jug fir
        CheckFeasibleHelper(jugs, L - jug.low,
                            H - jug.high)) { // after that, pick another jug
      return true;
    }
  }
  // we cannot use any jug to reach this L,H
  // mark as impossible
  cache.insert(cacheKey);
  return false;
}

bool CheckFeasible(const vector<Jug> &jugs, int L, int H) {
  cache.clear();
  return CheckFeasibleHelper(jugs, L, H);
}

template <> struct SerializationTraits<Jug> : UserSerTraits<Jug, int, int> {};

bool operator==(const Jug &lhs, const Jug &rhs) {
  return lhs.low == rhs.low && lhs.high == rhs.high;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"jugs", "L", "H"};
  return GenericTestMain(args, "defective_jugs.cc", "defective_jugs.tsv",
                         &CheckFeasible, DefaultComparator{}, param_names);
}
