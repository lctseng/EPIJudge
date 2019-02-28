#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <string>
using std::reverse;
using std::string;

void ReverseWords(string *sPtr) {
  if (!sPtr)
    return;
  string &s = *sPtr;
  // stage 1: full reverse
  reverse(s.begin(), s.end());
  // reverse every word
  int wordBegin = 0;
  int i = 0;
  while (i < s.length()) {
    if (s[i] == ' ') {
      // found a word, reverse it
      reverse(s.begin() + wordBegin, s.begin() + i);
      // skip next whitespaces
      while (i < s.length() - 1 && s[i + 1] == ' ')
        ++i;
      wordBegin = i + 1;
    }
    ++i;
  }
  // HANDLE final word
  reverse(s.begin() + wordBegin, s.end());
  return;
}
string ReverseWordsWrapper(TimedExecutor &executor, string s) {
  string s_copy = s;

  executor.Run([&] { ReverseWords(&s_copy); });

  return s_copy;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "s"};
  return GenericTestMain(args, "reverse_words.cc", "reverse_words.tsv",
                         &ReverseWordsWrapper, DefaultComparator{},
                         param_names);
}
