#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <vector>
using std::vector;

struct Event {
  int start, finish; // finish is inclusive!
};

struct TypeWithTime {
  bool isOpen;
  int time;
};

bool cmp(const TypeWithTime &a, const TypeWithTime &b) {
  if (a.time == b.time) {
    return a.isOpen && !b.isOpen; // open comes first
  } else {
    return a.time < b.time;
  }
}

int FindMaxSimultaneousEvents(const vector<Event> &A) {
  vector<TypeWithTime> typeWithTimeList;
  for (auto &event : A) {
    typeWithTimeList.push_back({true, event.start});
    typeWithTimeList.push_back({false, event.finish});
  }
  sort(typeWithTimeList.begin(), typeWithTimeList.end(), cmp);
  int maxValue = 0;
  int intervals = 0;
  for (auto &typeWithEvent : typeWithTimeList) {
    if (typeWithEvent.isOpen)
      intervals++;
    else
      intervals--;

    if (intervals > maxValue)
      maxValue = intervals;
  }

  return maxValue;
}
template <>
struct SerializationTraits<Event> : UserSerTraits<Event, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "calendar_rendering.cc",
                         "calendar_rendering.tsv", &FindMaxSimultaneousEvents,
                         DefaultComparator{}, param_names);
}
