#include "test_framework/generic_test.h"
#include <queue>
#include <vector>
using std::greater;
using std::priority_queue;
using std::vector;

vector<double> OnlineMedian(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator &sequence_end) {
  if (sequence_begin == sequence_end)
    return {};
  priority_queue<int, vector<int>, greater<int>> larger;
  priority_queue<int> smaller;
  bool isOdd = false;
  vector<double> res;
  while (sequence_begin != sequence_end) {
    int val = *(sequence_begin++);
    if (isOdd) {
      // push in small first
      smaller.push(val);
      int largeInSmaller = smaller.top();
      smaller.pop();
      larger.push(largeInSmaller);
      int smallInLarger = larger.top();
      larger.pop();
      smaller.push(smallInLarger);
    } else {
      // push in large
      larger.push(val);
      int smallInLarger = larger.top();
      larger.pop();
      smaller.push(smallInLarger);
      int largeInSmaller = smaller.top();
      smaller.pop();
      larger.push(largeInSmaller);
    }
    isOdd = !isOdd;
    if (isOdd) {
      res.push_back(larger.top());
    } else {
      res.push_back((larger.top() + smaller.top()) / 2.0);
    }
  }
  return res;
}
vector<double> OnlineMedianWrapper(const vector<int> &sequence) {
  return OnlineMedian(cbegin(sequence), cend(sequence));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "online_median.cc", "online_median.tsv",
                         &OnlineMedianWrapper, DefaultComparator{},
                         param_names);
}
