#include "test_framework/generic_test.h"
#include <vector>
using std::sort;
using std::vector;
bool HasThreeSum(vector<int> A, int t) {
  sort(A.begin(), A.end());
  int n = A.size();
  // can use same number twice!
  for (int i = 0; i < n; i++) {
    // pruning: if i too large, break;
    if (3 * A[i] > t)
      break;
    // pruning: if i too small, continue
    if (i >= 1 && A[i] + 2 * A[n - 1] < t)
      continue;
    int left = i, right = n - 1;
    while (left <= right) {
      int curSum = A[i] + A[left] + A[right];
      if (curSum < t) {
        left++;
      } else if (curSum > t) {
        right--;
      } else {
        return true;
      }
    }
  }
  // unable to find
  return false;
}

// Prev Backup
// bool HasThreeSum(vector<int> A, int t) {
//   sort(A.begin(), A.end());
//   // try each A[i]
//   int size = A.size();
//   for (int i = 0; i < size; i++) {
//     // pruning
//     if (3 * A[i] > t) // too large for all future numbers
//       return false;
//     if (A[i] + 2 * A[size - 1] < t) // too small for current i
//       continue;
//     // try two pointer
//     int newTarget = t - A[i];
//     int left = i, right = size - 1;
//     while (left <= right) {
//       if (A[left] + A[right] == newTarget)
//         return true;
//       else if (A[left] + A[right] < newTarget) {
//         ++left;
//         // skip duplicates
//         while (left < right && A[left - 1] == A[left])
//           ++left;
//       } else {
//         --right;
//         // skip duplicates
//         while (left < right && A[right + 1] == A[right])
//           --right;
//       }
//     }
//   }
//   return false;
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "t"};
  return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                         DefaultComparator{}, param_names);
}
