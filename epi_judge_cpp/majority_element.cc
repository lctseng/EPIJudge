#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

string MajoritySearch(vector<string>::const_iterator stream_begin,
                      const vector<string>::const_iterator stream_end) {
  auto curMaxIt = stream_begin++;
  int maxCount = 1;
  while (stream_begin != stream_end) {
    auto nextIt = stream_begin++;
    if (*nextIt == *curMaxIt)
      maxCount++;
    else {
      if (--maxCount < 0) {
        curMaxIt = nextIt;
        maxCount = 1;
      }
    }
  }
  return *curMaxIt;
}
string MajoritySearchWrapper(const vector<string> &stream) {
  return MajoritySearch(cbegin(stream), cend(stream));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream"};
  return GenericTestMain(args, "majority_element.cc", "majority_element.tsv",
                         &MajoritySearchWrapper, DefaultComparator{},
                         param_names);
}
