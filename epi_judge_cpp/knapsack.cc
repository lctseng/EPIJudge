#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::max;
using std::vector;

struct Item {
  int weight, value;
};

// consider items[index:] with capacity
int OptimumSubjectToCapacityHelper(vector<vector<int>> &dp,
                                   const vector<Item> &items, int capacity,
                                   int index) {
  if (capacity <= 0 || index >= items.size())
    return 0;
  int &dpEntry = dp[index][capacity];
  if (dpEntry >= 0)
    return dpEntry;
  int picked = items[index].weight <= capacity
                   ? OptimumSubjectToCapacityHelper(
                         dp, items, capacity - items[index].weight, index + 1) +
                         items[index].value
                   : 0;
  int notPicked =
      OptimumSubjectToCapacityHelper(dp, items, capacity, index + 1);
  return dpEntry = max(picked, notPicked);
}

int OptimumSubjectToCapacity(const vector<Item> &items, int capacity) {
  vector<vector<int>> dp(items.size());
  for (int i = 0; i < items.size(); i++) {
    dp[i].assign(capacity + 1, -1);
  }
  return OptimumSubjectToCapacityHelper(dp, items, capacity, 0);
}
template <> struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"items", "capacity"};
  return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
                         &OptimumSubjectToCapacity, DefaultComparator{},
                         param_names);
}
