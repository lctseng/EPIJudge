#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <deque>
#include <vector>
using std::deque;
using std::vector;

struct TrafficElement {
  bool operator==(const TrafficElement &that) const {
    return time == that.time && volume == that.volume;
  }
  int time;
  double volume;
};

vector<TrafficElement> CalculateTrafficVolumes(const vector<TrafficElement> &A,
                                               int w) {
  // mono deque
  vector<TrafficElement> res;
  deque<TrafficElement> maxTraffic;
  for (int i = 0; i < A.size(); i++) {
    // insert
    while (maxTraffic.size() && A[i].volume > maxTraffic.back().volume) {
      maxTraffic.pop_back();
    }
    maxTraffic.push_back(A[i]);
    // pop
    // remove out-of-window front
    int minTime = A[i].time - w;
    while (maxTraffic.size() && maxTraffic.front().time < minTime) {
      maxTraffic.pop_front();
    }
    // record
    res.push_back({A[i].time, maxTraffic.front().volume});
  }

  return res;
}
template <>
struct SerializationTraits<TrafficElement>
    : UserSerTraits<TrafficElement, int, double> {};

std::ostream &operator<<(std::ostream &out, const TrafficElement &te) {
  return out << '[' << te.time << ", " << te.volume << ']';
};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "w"};
  return GenericTestMain(args, "max_of_sliding_window.cc",
                         "max_of_sliding_window.tsv", &CalculateTrafficVolumes,
                         DefaultComparator{}, param_names);
}
