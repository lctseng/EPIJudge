#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>
using std::make_shared;
using std::shared_ptr;
shared_ptr<ListNode<int>> ListPivoting(const shared_ptr<ListNode<int>> &l,
                                       int x) {
  // three dummy head
  auto dummyLess = make_shared<ListNode<int>>(x - 1);
  auto dummyEqual = make_shared<ListNode<int>>(x);
  auto dummyGreater = make_shared<ListNode<int>>(x + 1);
  // iterate l
  auto currentLess = dummyLess, currentEqual = dummyEqual,
       currentGreater = dummyGreater;
  auto current = l;
  while (current) {
    if (current->data > x) {
      currentGreater->next = current;
      currentGreater = currentGreater->next;
    } else if (current->data < x) {
      currentLess->next = current;
      currentLess = currentLess->next;
    } else {
      // equal
      currentEqual->next = current;
      currentEqual = currentEqual->next;
    }
    current = current->next;
  }
  // BE CAREFUL!
  // fix tailing
  // combine these three, order MATTERS!
  currentGreater->next = nullptr;
  currentEqual->next = dummyGreater->next;
  currentLess->next = dummyEqual->next;
  return dummyLess->next;
}
std::vector<int> ListToVector(const shared_ptr<ListNode<int>> &l) {
  std::vector<int> v;
  ListNode<int> *it = l.get();
  while (it) {
    v.push_back(it->data);
    it = it->next.get();
  }
  return v;
}

void ListPivotingWrapper(TimedExecutor &executor,
                         const shared_ptr<ListNode<int>> &l, int x) {
  std::vector<int> original = ListToVector(l);

  std::shared_ptr<ListNode<int>> pivoted_list =
      executor.Run([&] { return ListPivoting(l, x); });

  std::vector<int> pivoted = ListToVector(pivoted_list);
  enum { kLess, kEq, kGreater } mode = kLess;
  for (auto &i : pivoted) {
    switch (mode) {
    case kLess:
      if (i == x) {
        mode = kEq;
      } else if (i > x) {
        mode = kGreater;
      }
      break;
    case kEq:
      if (i < x) {
        throw TestFailure("List is not pivoted");
      } else if (i > x) {
        mode = kGreater;
      }
      break;
    case kGreater:
      if (i <= x) {
        throw TestFailure("List is not pivoted");
      }
    }
  }
  std::sort(begin(original), end(original));
  std::sort(begin(pivoted), end(pivoted));
  if (original != pivoted) {
    throw TestFailure("Result list contains different values");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l", "x"};
  return GenericTestMain(args, "pivot_list.cc", "pivot_list.tsv",
                         &ListPivotingWrapper, DefaultComparator{},
                         param_names);
}
