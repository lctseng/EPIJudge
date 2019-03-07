#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::sort;
using std::vector;

struct Interval {
  int left, right;
};

bool cmpInterval(const Interval &a, const Interval &b) {
  return a.right < b.right;
}

int FindMinimumVisits(vector<Interval> intervals) {
  // sort according to end time
  int count = 0;
  int i = 0;
  while (i < intervals.size()) {
    // pick the end point of this task
    ++count;
    int endPoint = intervals[i++].right;
    // if possible, advance this check point and see how far we can reach
    // use start time!
    while (i < intervals.size() && intervals[i].left <= endPoint)
      ++i;
  }
  return count;
}
template <>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"intervals"};
  return GenericTestMain(args, "minimum_points_covering_intervals.cc",
                         "minimum_points_covering_intervals.tsv",
                         &FindMinimumVisits, DefaultComparator{}, param_names);
}
