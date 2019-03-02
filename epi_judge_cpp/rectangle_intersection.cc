#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <tuple>
using std::min;
using std::swap;

struct Rect {
  int x, y, width, height;
};

Rect IntersectRectangle(const Rect &R1, const Rect &R2) {
  const Rect *left = &R1;
  const Rect *right = &R2;
  if (left->x > right->x) {
    swap(left, right);
  }
  // x-axis non-overlap check?
  if (left->x + left->width < right->x)
    return {0, 0, -1, -1};
  const Rect *top = &R1;
  const Rect *bottom = &R2;
  if (bottom->y > top->y) {
    swap(bottom, top);
  }
  // y-axis non-overlap check?
  if (bottom->y + bottom->height < top->y) {
    return {0, 0, -1, -1};
  }
  // must overlaps
  // Note: Be careful the max height and max width!
  int width = min(min(R1.width, R2.width), left->x + left->width - right->x);
  int height =
      min(min(R1.height, R2.height), bottom->y + bottom->height - top->y);
  return {right->x, top->y, width, height};
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
