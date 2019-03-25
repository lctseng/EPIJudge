#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <tuple>
using std::max;
using std::min;
using std::swap;

struct Rect {
  int x, y, width, height;
};

Rect IntersectRectangle(const Rect &R1, const Rect &R2) {
  // examine X and Y respectively
  auto RightEnd = [&](const Rect &R) { return R.x + R.width; };
  auto TopEnd = [&](const Rect &R) { return R.y + R.height; };
  do {
    // horz check
    const auto &rightRect = RightEnd(R1) > RightEnd(R2) ? R1 : R2;
    const auto &leftRect = &rightRect == &R1 ? R2 : R1;
    if (rightRect.x > RightEnd(leftRect))
      break;
    // vertical check
    const auto &upperRect = TopEnd(R1) > TopEnd(R2) ? R1 : R2;
    const auto &lowerRect = &upperRect == &R1 ? R2 : R1;
    if (upperRect.y > TopEnd(lowerRect))
      break;
    // now the two rect do overlap
    int newX = max(R1.x, R2.x);
    int newY = max(R1.y, R2.y);
    int newTopEnd = min(TopEnd(R1), TopEnd(R2));
    int newRightEnd = min(RightEnd(R1), RightEnd(R2));
    return {newX, newY, newRightEnd - newX, newTopEnd - newY};
    // return final result
  } while (false);
  // early terminate falls into default result: empty
  return {0, 0, -1, -1};
}

bool operator==(const Rect &r1, const Rect &r2) {
  return std::tie(r1.x, r1.y, r1.width, r1.height) ==
         std::tie(r2.x, r2.y, r2.width, r2.height);
}

template <>
struct SerializationTraits<Rect> : UserSerTraits<Rect, int, int, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
    return {FmtStr("area({})", arg_name), FmtStr("perimeter({})", arg_name),
            FmtStr("max(w, h)({})", arg_name)};
  }

  static std::vector<int> GetMetrics(const Rect &x) {
    return {x.height * x.width, 2 * (x.height + x.width),
            std::max(x.height, x.width)};
  }
};

std::ostream &operator<<(std::ostream &out, const Rect &r) {
  return PrintTo(out, std::make_tuple(r.x, r.y, r.width, r.height));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"R1", "R2"};
  return GenericTestMain(args, "rectangle_intersection.cc",
                         "rectangle_intersection.tsv", &IntersectRectangle,
                         DefaultComparator{}, param_names);
}
