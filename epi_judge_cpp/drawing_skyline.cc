#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <set>
#include <vector>
using std::greater;
using std::max;
using std::min;
using std::multiset;
using std::vector;

struct Rect {
  int left, right, height;
};
typedef vector<Rect> Skyline;

struct Event {
  bool isOpen;
  int position;
  int height;
};

bool cmpEvent(const Event &a, const Event &b) {
  if (a.position == b.position) {
    return !a.isOpen && b.isOpen;
  } else {
    return a.position < b.position;
  }
}

// When max width W is constant and small
Skyline ComputeSkyline(const vector<Rect> &buildings) {
  // compute offset
  int leftOffset = 2000000000;
  int rightBound = 0;
  for (auto &rect : buildings) {
    if (rect.left < leftOffset) {
      leftOffset = rect.left;
    }
    if (rect.right > rightBound) {
      rightBound = rect.right;
    }
  }
  // leftOffset is the smallest left
  // create digital array size = rightBound - leftOffset
  vector<int> plane(rightBound - leftOffset + 1, 0);
  for (auto &building : buildings) {
    for (int i = building.left; i <= building.right; i++) {
      plane[i - leftOffset] = max(plane[i - leftOffset], building.height);
    }
  }
  Skyline res;
  // when adding answer, remenber to add offset
  int prevI = 0;
  for (int i = 0; i < plane.size(); i++) {
    if (i == plane.size() - 1 || plane[i] != plane[i + 1]) {
      // create for current
      res.push_back({leftOffset + prevI, leftOffset + i, plane[i]});
      prevI = i + 1;
    }
  }
  return res;
}

Skyline ComputeSkylineGeneral(const vector<Rect> &buildings) {
  if (buildings.empty())
    return {};
  // generate set of events from endpoints: open, close events
  vector<Event> events;
  for (auto &rect : buildings) {
    events.push_back({true, rect.left, rect.height});
    events.push_back({false, rect.right + 1, rect.height});
  }
  // sort events according to coordinate
  sort(events.begin(), events.end(), cmpEvent);
  // scan over all event
  // maintain a set of active building
  Skyline res;
  multiset<int, greater<int>> activeHeight;
  int prevPosition = events[0].position;
  for (int i = 0; i < events.size(); i++) {
    if (events[i].position != prevPosition) {
      // perform add skyline
      int currentH = activeHeight.size() ? *(activeHeight.begin()) : 0;
      // when get to new position, create previous skyline
      int rightPosition = events[i].position - 1;
      Rect rect = {prevPosition, rightPosition, currentH};
      // merge with existing if possible
      if (res.size() && res.back().height == currentH) {
        // merge
        res.back().right = rightPosition;
      } else {
        res.push_back(rect);
      }
      prevPosition = events[i].position;
    }
    // process event
    if (events[i].isOpen) {
      activeHeight.insert(events[i].height);
    } else {
      // BE CAREFUL!
      // avoid multiple erasing!
      activeHeight.erase(activeHeight.find(events[i].height));
    }
  }
  // no last update needed
  return res;
}
bool operator==(const Rect &a, const Rect &b) {
  return a.left == b.left && a.right == b.right && a.height == b.height;
}

template <>
struct SerializationTraits<Rect> : UserSerTraits<Rect, int, int, int> {};

std::ostream &operator<<(std::ostream &out, const Rect &r) {
  return PrintTo(out, std::make_tuple(r.left, r.right, r.height));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"buildings"};
  return GenericTestMain(args, "drawing_skyline.cc", "drawing_skyline.tsv",
                         &ComputeSkyline, DefaultComparator{}, param_names);
}
