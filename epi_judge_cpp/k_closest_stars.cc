#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_utils.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>
using std::priority_queue;
using std::vector;

struct Star {
  bool operator<(const Star &that) const {
    return DistanceSquare() < that.DistanceSquare();
  }

  double Distance() const { return sqrt(DistanceSquare()); }
  double DistanceSquare() const { return x * x + y * y + z * z; }

  double x, y, z;
};

vector<Star> FindClosestKStars(vector<Star>::const_iterator stars_begin,
                               const vector<Star>::const_iterator &stars_end,
                               int k) {
  priority_queue<Star> pq;
  while (stars_begin != stars_end) {
    auto &star = *(stars_begin++);
    if (pq.size() < k) {
      pq.push(star);
    } else {
      if (star.DistanceSquare() < pq.top().DistanceSquare()) {
        pq.push(star);
        pq.pop();
      }
    }
  }
  // store answer into vector
  vector<Star> res;
  while (pq.size()) {
    res.push_back(pq.top());
    pq.pop();
  }
  return res;
}
template <>
struct SerializationTraits<Star> : UserSerTraits<Star, double, double, double> {
};

std::ostream &operator<<(std::ostream &out, const Star &s) {
  return out << s.Distance();
}

bool Comp(const vector<double> &expected, vector<Star> output) {
  if (output.size() != expected.size()) {
    return false;
  }

  std::sort(begin(output), end(output));

  for (int i = 0; i < output.size(); ++i) {
    if (!DefaultComparator()(output[i].Distance(), expected[i])) {
      return false;
    }
  }
  return true;
}

vector<Star> FindClosestKStarsWrapper(const vector<Star> &stars, int k) {
  return FindClosestKStars(cbegin(stars), cend(stars), k);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stars", "k"};
  return GenericTestMain(args, "k_closest_stars.cc", "k_closest_stars.tsv",
                         &FindClosestKStarsWrapper, &Comp, param_names);
}
