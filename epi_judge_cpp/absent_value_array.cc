#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include <stdexcept>
#include <vector>
using std::invalid_argument;
using std::vector;

const int MAX_COUNT = 1 << 16;
const unsigned MASK = 0xFFFF;

int FindMissingElement(vector<int>::const_iterator stream_begin,
                       const vector<int>::const_iterator &stream_end) {
  int count[MAX_COUNT] = {0};

  auto begin = stream_begin;
  while (begin != stream_end) {
    int value = *(begin++);
    int key = value >> 16;
    count[key]++;
  }
  for (int i = 0; i < MAX_COUNT; i++) {
    if (count[i] < MAX_COUNT) {
      // IP begins with "i" contains missing value
      bool count2[MAX_COUNT] = {0};
      auto begin2 = stream_begin;
      while (begin2 != stream_end) {
        int value = *(begin2++);
        if ((value >> 16) == i) {
          count2[value & MASK] = true;
        }
      }
      // scan count2, find the first false
      for (int j = 0; j < MAX_COUNT; j++) {
        if (!count2[j]) {
          return (i << 16) | j;
        }
      }
    }
  }
  return 0;
}
int FindMissingElementWrapper(const vector<int> &stream) {
  try {
    return FindMissingElement(cbegin(stream), cend(stream));
  } catch (invalid_argument &) {
    throw TestFailure("Unexpected no_missing_element exception");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream"};
  return GenericTestMain(args, "absent_value_array.cc",
                         "absent_value_array.tsv", &FindMissingElementWrapper,
                         DefaultComparator{}, param_names);
}
