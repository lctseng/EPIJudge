#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
#include <vector>
using std::max;
using std::vector;

struct Interval {
  struct Endpoint {
    bool is_closed;
    int val;
  };

  Endpoint left, right;
};

bool isOverlap(const Interval &a, const Interval &b) {
  if (a.right.is_closed) {
    return a.right.val >= b.left.val;
  } else {
    // endpoint is open
    return b.left.is_closed ? a.right.val >= b.left.val
                            : a.right.val > b.left.val;
  }
}

void mergeIntervalRight(Interval &a, Interval &b) {
  if (a.right.is_closed == b.right.is_closed) {
    a.right.val = max(a.right.val, b.right.val);
  } else {
    if (a.right.is_closed) {
      // b is open
      if (b.right.val > a.right.val) {
        a.right = b.right;
      }
    } else {
      // a is open
      if (b.right.val >= a.right.val) {
        a.right = b.right;
      }
    }
  }
}

bool cmp(const Interval &a, const Interval &b) {
  if (a.left.val == b.left.val) {
    return a.left.is_closed && !b.left.is_closed;
  } else {
    return a.left.val < b.left.val;
  }
}

vector<Interval> UnionOfIntervals(vector<Interval> intervals) {
  sort(intervals.begin(), intervals.end(), cmp);
  vector<Interval> res;
  int i = 0;
  int n = intervals.size();
  while (i < n) {
    int j = i + 1;
    while (j < n && isOverlap(intervals[i], intervals[j])) {
      mergeIntervalRight(intervals[i], intervals[j]);
      ++j;
    }
    res.push_back(intervals[i]);
    i = j;
  }
  return res;
}
struct FlatInterval {
  int left_val;
  bool left_is_closed;
  int right_val;
  bool right_is_closed;

  FlatInterval(int left_val, bool left_is_closed, int right_val,
               bool right_is_closed)
      : left_val(left_val), left_is_closed(left_is_closed),
        right_val(right_val), right_is_closed(right_is_closed) {}

  explicit FlatInterval(Interval in)
      : left_val(in.left.val), left_is_closed(in.left.is_closed),
        right_val(in.right.val), right_is_closed(in.right.is_closed) {}

  operator Interval() const {
    return {{left_is_closed, left_val}, {right_is_closed, right_val}};
  }

  bool operator==(const FlatInterval &rhs) const {
    return std::tie(left_val, left_is_closed, right_val, right_is_closed) ==
           std::tie(rhs.left_val, rhs.left_is_closed, rhs.right_val,
                    rhs.right_is_closed);
  }
};

template <>
struct SerializationTraits<FlatInterval>
    : UserSerTraits<FlatInterval, int, bool, int, bool> {};

std::ostream &operator<<(std::ostream &out, const FlatInterval &i) {
  return out << (i.left_is_closed ? '<' : '(') << i.left_val << ", "
             << i.right_val << (i.right_is_closed ? '>' : ')');
}

std::vector<FlatInterval>
UnionOfIntervalsWrapper(TimedExecutor &executor,
                        const std::vector<FlatInterval> &intervals) {
  std::vector<Interval> casted;
  for (const FlatInterval &i : intervals) {
    casted.push_back(static_cast<Interval>(i));
  }

  std::vector<Interval> result =
      executor.Run([&] { return UnionOfIntervals(casted); });

  return {begin(result), end(result)};
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "intervals"};
  return GenericTestMain(args, "intervals_union.cc", "intervals_union.tsv",
                         &UnionOfIntervalsWrapper, DefaultComparator{},
                         param_names);
}
