#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <iterator>
#include <string>
#include <vector>
using std::string;
using std::vector;

int ReplaceAndRemove(int size, char s[]) {
  // stage 1: remove all b
  int j = 0;
  int numA = 0;
  for (int i = 0; i < size; i++) {
    if (s[i] != 'b') {
      s[j++] = s[i];
      if (s[i] == 'a')
        numA++;
    }
  }
  // j is the final size after removing b
  // stage 2: convert a to dd
  int finalSize = j + numA;
  int finalIndex = finalSize - 1;
  for (int i = j - 1; i >= 0; i--) {
    if (s[i] == 'a') {
      s[finalIndex--] = 'd';
      s[finalIndex--] = 'd';
    } else {
      s[finalIndex--] = s[i];
    }
  }
  return finalSize;
}
vector<string> ReplaceAndRemoveWrapper(TimedExecutor &executor, int size,
                                       const vector<string> &s) {
  std::vector<char> s_copy(s.size(), '\0');
  for (int i = 0; i < s.size(); ++i) {
    if (!s[i].empty()) {
      s_copy[i] = s[i][0];
    }
  }

  int res_size =
      executor.Run([&] { return ReplaceAndRemove(size, s_copy.data()); });

  vector<string> result;
  for (int i = 0; i < res_size; ++i) {
    result.emplace_back(string(1, s_copy[i]));
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "size", "s"};
  return GenericTestMain(args, "replace_and_remove.cc",
                         "replace_and_remove.tsv", &ReplaceAndRemoveWrapper,
                         DefaultComparator{}, param_names);
}
