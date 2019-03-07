#include "test_framework/generic_test.h"
#include <vector>
using std::sort;
using std::vector;

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
