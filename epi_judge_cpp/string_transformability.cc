#include "test_framework/generic_test.h"
#include <string>
#include <unordered_set>
using std::string;
using std::unordered_set;

// uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string &s, const string &t) {
  if (s == t)
    return 0;
  // two-ended BFS
  // two explored set, iterate short set
  unordered_set<string> head, tail;
  unordered_set<string> *pHead = &head, *pTail = &tail;
  head.insert(s);
  tail.insert(t);
  int step = 1;
  while (head.size() && tail.size()) {
    // ensure head is smaller
    if (pHead->size() > pTail->size()) {
      swap(pHead, pTail);
    }
    unordered_set<string> nextHead;
    // extend pHead set
    for (auto &src : *pHead) {
      string current = src; // update this current is explore
      for (int i = 0; i < current.length(); i++) {
        // try change this
        char origC = current[i];
        for (char c = 'a'; c <= 'z'; c++) {
          if (c != origC) {
            current[i] = c;
            // find in dict
            if (D.count(current)) {
              // check tail
              if (pTail->count(current)) {
                return step;
              }
              nextHead.insert(current);
            }
          }
        }
        // restore this
        current[i] = origC;
      }
    }
    // advance
    // prevent going back: remove all is pHead
    for (auto &str : *pHead) {
      D.erase(str);
    }
    *pHead = move(nextHead);
    step++;
  }

  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"D", "s", "t"};
  return GenericTestMain(args, "string_transformability.cc",
                         "string_transformability.tsv", &TransformString,
                         DefaultComparator{}, param_names);
}
