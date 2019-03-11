#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int BinarySearchUnknownLength(const vector<int> &A, int k) {
  if (A.empty())
    return -1;
  // find interval such that A[2^p-1] <= k <= A[2^(p+1)-1]
  // upper bound can be an exception
  int p = 0;
  while (true) {
    // try this p
    int low = (1 << p) - 1;
    int high = (1 << (p + 1)) - 1;
    // check low is valid
    if (A[low] == k)
      return low;
    if (A[low] < k) {
      bool found = false;
      // check high
      try {
        int highVal = A.at(high);
        if (highVal < k) {
          // still too small
        } else if (highVal > k) {
          // high is enough
          found = true;
        } else {
          return high;
        }
      } catch (...) {
        // high is high enough
        found = true;
      }
      if (found)
        break;
    } else {
      // low is already too high
      --p;
      break;
    }
    ++p;
  }
  // ensure p>=0
  if (p < 0)
    p = 0;
  int low = (1 << p) - 1;
  int high = (1 << (p + 1)) - 1;
  // stage 2: use that length to do binary search
  // Note that if the mid element access with exception, force go left
  while (low < high) {
    int mid = low + (high - low) / 2;
    try {
      int val = A.at(mid);
      if (val > k) {
        // too large
        high = mid;
      } else if (val < k) {
        // too small
        low = mid + 1;
      } else {
        // found
        return mid;
      }
    } catch (...) {
      // mid too large
      high = mid;
    }
  }
  return -1;
}

int BinarySearchUnknownLengthDumb(const vector<int> &A, int k) {
  if (A.empty())
    return -1; // we do not handle the empty Array case
  // stage 1: find length using binary search
  // BE CAREFUL!
  // init low and high
  int high = 1, low = 0;
  bool found = false;
  while (!found) {
    // try access
    try {
      int val = A.at(high);
      // EXTRA STEP: look at the value!
      if (val > k) {
        found = true;
      } else if (val < k) {
        // BE CAREFUL!!
        // the direction of shift!
        high <<= 1;
      } else {
        // high is k
        return high;
      }
    } catch (...) {
      // too large, stop
      found = true;
    }
  }
  // EXTRA STEP: make low higher
  found = false;
  while (!found && low < high) {
    try {
      int val = A.at(low);
      if (val < k) {
        // go higher!
        if (!low)
          low = 1;
        else
          low <<= 1;
      } else if (val > k) {
        // low too high
        found = true;
        // restore to prev
        low >>= 1;
      } else {
        // we found...
        return low;
      }
    } catch (...) {
      // low too high
      found = true;
      low >>= 1;
    }
  }
  if (low >= high) {
    low >>= 1;
  }
  // stage 2: use that length to do binary search
  // Note that if the mid element access with exception, force go left
  while (low < high) {
    int mid = low + (high - low) / 2;
    try {
      int val = A.at(mid);
      if (val > k) {
        // too large
        high = mid;
      } else if (val < k) {
        // too small
        low = mid + 1;
      } else {
        // found
        return mid;
      }
    } catch (...) {
      // mid too large
      high = mid;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(
      args, "search_unknown_length_array.cc", "search_unknown_length_array.tsv",
      &BinarySearchUnknownLength, DefaultComparator{}, param_names);
}
