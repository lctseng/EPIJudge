#include "test_framework/generic_test.h"
#include <vector>
using std::sort;
using std::vector;

double FindSalaryCapBinarySearch(int target_payroll,
                                 vector<int> current_salaries) {
  sort(current_salaries.begin(), current_salaries.end());
  vector<int> prefixSum(1, 0);
  for (int val : current_salaries) {
    prefixSum.push_back(prefixSum.back() + val);
  }
  // perform binary search
  int begin = 0, end = current_salaries.size();
  while (begin < end) {
    // try use this?
    int mid = begin + (end - begin) / 2;
    double remainBudge = target_payroll - prefixSum[mid];
    double divided = remainBudge / (current_salaries.size() - mid);
    if (divided <= current_salaries[mid] &&
        (mid == 0 || divided > current_salaries[mid - 1])) {
      return divided;
    } else if (divided > current_salaries[mid]) {
      // go right
      begin = mid + 1;
    } else {
      // go left
      end = mid;
    }
  }
  // not found
  return -1;
}

double FindSalaryCap(int target_payroll, vector<int> current_salaries) {
  sort(current_salaries.begin(), current_salaries.end());
  double remainBudge = target_payroll;
  for (int i = 0; i < current_salaries.size(); i++) {
    int remainPeople = current_salaries.size() - i;
    double divided = remainBudge / remainPeople;
    if (divided <= current_salaries[i]) {
      return divided;
    }
    remainBudge -= current_salaries[i];
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"target_payroll", "current_salaries"};
  return GenericTestMain(args, "find_salary_threshold.cc",
                         "find_salary_threshold.tsv", &FindSalaryCap,
                         DefaultComparator{}, param_names);
}
