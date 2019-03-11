#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <climits>
#include <vector>
using std::max;
using std::min;
using std::swap;
using std::vector;

struct HighwaySection {
  int x, y, distance;
};

HighwaySection FindBestProposals(const vector<HighwaySection> &H,
                                 const vector<HighwaySection> &P, int n) {
  // apply FW first: O(n^3)
  // to save time: the distance valid for i -> j where i < j
  vector<vector<int>> distance(n, vector<int>(n, INT_MAX));
  // init FW
  for (int i = 0; i < n; i++) {
    distance[i][i] = 0;
  }
  // insert known distance
  auto getDistanceEntry = [&](int i, int j) -> int & {
    if (i > j) {
      return distance[j][i];
    } else {
      return distance[i][j];
    }
  };
  for (auto &section : H) {
    getDistanceEntry(section.x, section.y) = section.distance;
  }
  // start dp
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - 1; i++) {
      for (int j = i + 1; j < n; j++) {
        // go through k
        int ik = getDistanceEntry(i, k);
        if (ik == INT_MAX)
          continue;
        int kj = getDistanceEntry(k, j);
        if (kj == INT_MAX)
          continue;
        int &ij = getDistanceEntry(i, j);
        if (ik + kj < ij) {
          ij = ik + kj;
        }
      }
    }
  }
  // for every proposal, try every pair that walk through it
  // O(k*n^2)
  HighwaySection maxSection;
  int maxImprove = 0;
  for (auto &newSection : P) {
    int currentImprove = 0;
    // for every i, j, try new path: i->newSection.x->newSection.y->j
    // pick i->x or i->y the smaller one
    for (int i = 0; i < n - 1; i++) {
      int ix = getDistanceEntry(i, newSection.x);
      int iy = getDistanceEntry(i, newSection.y);
      int from = newSection.x;
      int to = newSection.y;
      if (ix > iy) {
        swap(from, to);
      }
      // precomputed // i->from->to
      int distanceNew = getDistanceEntry(i, from) + newSection.distance;
      for (int j = i + 1; j < n; j++) {
        // compute i->from->to->j by add to->j
        int diff =
            getDistanceEntry(i, j) - (distanceNew + getDistanceEntry(to, j));
        if (diff > 0) {
          currentImprove += diff;
        }
      }
    }
    if (currentImprove > maxImprove) {
      maxSection = newSection;
      maxImprove = currentImprove;
    }
  }
  return maxSection;
}

template <>
struct SerializationTraits<HighwaySection>
    : UserSerTraits<HighwaySection, int, int, int> {};

bool operator==(const HighwaySection &lhs, const HighwaySection &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.distance == rhs.distance;
}

std::ostream &operator<<(std::ostream &out, const HighwaySection &hs) {
  return PrintTo(out, std::make_tuple(hs.x, hs.y, hs.distance));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"H", "P", "n"};
  return GenericTestMain(args, "road_network.cc", "road_network.tsv",
                         &FindBestProposals, DefaultComparator{}, param_names);
}
