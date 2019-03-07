#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::lower_bound;
using std::max;
using std::min;
using std::move;
using std::next;
using std::prev;
using std::upper_bound;
using std::vector;

struct Interval {
  int left, right;
};

vector<Interval> AddInterval(const vector<Interval> &disjoint_intervals,
                             Interval new_interval) {
  auto cmpLeft = [](const Interval &a, const Interval &b) {
    return a.left < b.left;
  };
  auto cmpRight = [](const Interval &a, const Interval &b) {
    return a.right < b.right;
  };
  // find first interval I with I.right >= new.left
  auto it_I =
      lower_bound(disjoint_intervals.begin(), disjoint_intervals.end(),
                  Interval({new_interval.left, new_interval.left}), cmpRight);
  // if not found(it == end), new is at end
  if (it_I == disjoint_intervals.end()) {
    vector<Interval> res = disjoint_intervals;
    res.push_back(new_interval);
    return move(res);
  }
  // if found, do overlap check: I.left <= new.right
  if (it_I->left > new_interval.right) {
    // if not overlap, new interval is disjoint: concat: [:I) + new + [I:]
    vector<Interval> res(disjoint_intervals.begin(), it_I);
    res.push_back(new_interval);
    res.insert(res.end(), it_I, disjoint_intervals.end());
    return move(res);
  }
  // if overlap: start from I
  // find pass-over-1 interval K where J.left > new.right
  // go backward, get the last overlapped interval J
  auto it_J = prev(
      upper_bound(it_I, disjoint_intervals.end(),
                  Interval({new_interval.right, new_interval.right}), cmpLeft));
  new_interval.left = min(new_interval.left, it_I->left);
  new_interval.right = max(new_interval.right, it_J->right);
  // new.left = min(I.left, new.left), new.right = max(J.right, new.right)
  // concat [:I) + new + (J:]
  vector<Interval> res(disjoint_intervals.begin(), it_I);
  res.push_back(new_interval);
  res.insert(res.end(), next(it_J), disjoint_intervals.end());
  return move(res);
}
template <>
struct SerializationTraits<Interval> : UserSerTraits<Interval, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {FmtStr("length({})", arg_name)};
  }

  static std::vector<int> GetMetrics(const Interval &x) {
    return {x.right - x.left};
  }
};

bool operator==(const Interval &a, const Interval &b) {
  return a.left == b.left && a.right == b.right;
}

std::ostream &operator<<(std::ostream &out, const Interval &i) {
  return PrintTo(out, std::make_tuple(i.left, i.right));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"disjoint_intervals", "new_interval"};
  return GenericTestMain(args, "interval_add.cc", "interval_add.tsv",
                         &AddInterval, DefaultComparator{}, param_names);
}
