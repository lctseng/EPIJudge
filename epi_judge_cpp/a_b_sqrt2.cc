#include "test_framework/generic_test.h"
#include <limits>
#include <set>
#include <vector>
using std::max;
using std::min;
using std::numeric_limits;
using std::set;
using std::vector;

// Fast O(n) way
vector<double> GenerateFirstKABSqrt2(int k) {
  // like ugly number
  // maintain index for a and b
  // increment k times
  // edge case: k = 0
  // need to implement double Compare
  if (k == 0)
    return {};
  if (k == 1)
    return {0};
  const double kSqrt2 = sqrt(2);
  auto Compare = [](double a, double b) {
    double eps = std::numeric_limits<double>::epsilon();
    double diff = (a - b) / (max(abs(a), abs(b)));
    if (diff > eps) {
      return 1;
    } else if (diff < -eps) {
      return -1;
    } else {
      return 0;
    }
  };
  int a = 0, b = 0;
  vector<double> res = {0};
  for (int i = 1; i < k; i++) {
    double nextVal = min(res[a] + 1, res[b] + kSqrt2);
    res.push_back(nextVal);
    if (Compare(nextVal, res[a] + 1) == 0) {
      ++a;
    }
    if (Compare(nextVal, res[b] + kSqrt2) == 0) {
      ++b;
    }
  }
  return res;
}

// int Compare(double a, double b) {
//   double diff = (a - b) / max(abs(a), abs(b)); // THIS IS IMPORTANT!
//   if (abs(diff) < numeric_limits<double>::epsilon()) {
//     return 0;
//   } else if (diff < -numeric_limits<double>::epsilon()) {
//     return -1;
//   } else {
//     return 1;
//   }
// }

// const double kSqrt2 = sqrt(2);

// struct Number {
//   int a;
//   int b;
//   Number(int a, int b) : a(a), b(b) {}
//   double val() const { return a + b * kSqrt2; }
// };

// bool operator<(const Number &a, const Number &b) {
//   return Compare(a.val(), b.val()) == -1;
// }

// vector<double> GenerateFirstKABSqrt2(int k) { // BST way
//   set<Number> tree;
//   tree.emplace(0, 0);
//   vector<double> res;
//   for (int i = 0; i < k; i++) {
//     // extract smallest
//     auto smallIt = tree.begin();
//     Number small = *smallIt;
//     tree.erase(smallIt);
//     res.push_back(small.val());
//     // insert next two
//     tree.emplace(small.a + 1, small.b);
//     tree.emplace(small.a, small.b + 1);
//   }
//   return res;
// }

// vector<double> FastGenerateFirstKABSqrt2(int k) {
//   // just like ugly number on LeetCode
//   int a = 0, b = 0;
//   double sqrt2 = sqrt(2);
//   vector<double> res = {0};
//   for (int i = 1; i < k; i++) {
//     res.push_back(min(res[a] + 1, res[b] + sqrt2));
//     if (Compare(res.back(), res[a] + 1) == 0)
//       ++a;
//     if (Compare(res.back(), res[b] + sqrt2) == 0)
//       ++b;
//   }
//   res.resize(k);
//   return res;
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k"};
  return GenericTestMain(args, "a_b_sqrt2.cc", "a_b_sqrt2.tsv",
                         &GenerateFirstKABSqrt2, DefaultComparator{},
                         param_names);
}
