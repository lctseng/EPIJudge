#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::gcd;
using std::hash;
using std::make_pair;
using std::max;
using std::pair;
using std::size;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct Point {
  int x, y;
};

struct Line {
  pair<int, int> slope;
  pair<int, int> intercept;
};

pair<int, int> makeSimplestPair(int a, int b) {
  int common = gcd(a, b);
  // divide
  if (common != 0) {
    a /= common;
    b /= common;
  }
  // sign fix
  if (b < 0) {
    a = -a, b = -b;
  }
  return make_pair(a, b);
}

Line ComputeLineForTwoPoint(const Point &p1, const Point &p2) {
  Line l;
  int x_diff = p2.x - p1.x;
  int y_diff = p2.y - p1.y;
  if (x_diff == 0) {
    // verticle line
    l.slope = make_pair(1, 0);
    l.intercept = make_pair(p1.x, 0);
  } else {
    l.slope = makeSimplestPair(y_diff, x_diff);
    int upper = p2.x * p1.y - p1.x * p2.y;
    l.intercept = makeSimplestPair(upper, x_diff);
  }
  return l;
}

bool operator==(const Line &l1, const Line &l2) {
  return l1.slope == l2.slope && l1.intercept == l2.intercept;
}

template <> struct hash<pair<int, int>> {
  size_t operator()(const pair<int, int> &p) const {
    return (hash<int>{}(p.first) << 1) ^ hash<int>{}(p.second);
  }
};

template <> struct hash<Line> {
  size_t operator()(const Line &L) const {
    return (hash<pair<int, int>>{}(L.slope) << 1) ^
           (hash<pair<int, int>>{}(L.intercept));
  }
};

int FindLineWithMostPoints(const vector<Point> &points) {
  if (points.size() <= 2)
    return points.size();
  unordered_map<Line, unordered_set<int>> lineToPoints;
  // for very two pair of point, compute y = ax + b line, store into hashMap
  for (int i = 0; i < size(points) - 1; i++) {
    for (int j = i + 1; j < size(points); j++) {
      auto &pointSet =
          lineToPoints[ComputeLineForTwoPoint(points[i], points[j])];
      pointSet.insert(i);
      pointSet.insert(j);
    }
  }
  int maxValue = 0;
  for (auto &data : lineToPoints) {
    maxValue = max(maxValue, (int)(data.second.size()));
  }
  return maxValue;
}
template <>
struct SerializationTraits<Point> : UserSerTraits<Point, int, int> {};

bool operator==(const Point &lhs, const Point &rhs) {
  return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"points"};
  return GenericTestMain(
      args, "line_through_most_points.cc", "line_through_most_points.tsv",
      &FindLineWithMostPoints, DefaultComparator{}, param_names);
}
