#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <memory>
using std::shared_ptr;

int Length(shared_ptr<ListNode<int>> head) {
  int r = 0;
  while (head) {
    ++r;
    head = head->next;
  }
  return r;
}

shared_ptr<ListNode<int>> advanceByK(shared_ptr<ListNode<int>> head, int k) {
  while (k-- > 0) {
    head = head->next;
  }
  return head;
}

shared_ptr<ListNode<int>>
OverlappingNoCycleLists(shared_ptr<ListNode<int>> l0,
                        shared_ptr<ListNode<int>> l1) {
  // advance to list to the same length
  int len0 = Length(l0), len1 = Length(l1);
  if (len0 > len1) {
    l0 = advanceByK(l0, len0 - len1);
  } else if (len0 < len1) {
    l1 = advanceByK(l1, len1 - len0);
  }
  // now see can we reach same point?
  while (l0 && l1 && l0 != l1) {
    l0 = l0->next;
    l1 = l1->next;
  }
  return l0;
}

shared_ptr<ListNode<int>>
OverlappingNoCycleListsModified(shared_ptr<ListNode<int>> l0,
                                shared_ptr<ListNode<int>> l1) {
  // ensure two list exists
  if (!l0 || !l1)
    return nullptr;
  // edge case: fully overlapped
  if (l0 == l1)
    return l0;
  // link L1's tail to its head
  // traverse L0, if meet null, then no overlap
  // if meed L1's head, is overlap
  // Time: O(L0+L1), Space: O(1)
  // step 1: link L1
  auto tail1 = l1;
  while (tail1->next) {
    tail1 = tail1->next;
  }
  tail1->next = l1;
  // step 2: perform cycle detection
  auto slow = l0, fast = l0;
  shared_ptr<ListNode<int>> res = nullptr;
  bool hasCycle = false;
  while (true) {
    slow = slow->next;
    fast = fast->next;
    if (!fast)
      break;
    fast = fast->next;
    if (!fast)
      break;
    if (fast == slow) {
      hasCycle = true;
      break;
    }
  }
  if (hasCycle) {
    auto finder = l0;
    while (true) {
      slow = slow->next;
      finder = finder->next;
      if (slow == finder) {
        res = slow;
        break;
      }
    }
  }
  // remember to restore
  tail1->next = nullptr;
  return res;
  return nullptr;
}
void OverlappingNoCycleListsWrapper(TimedExecutor &executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1", "common"};
  return GenericTestMain(
      args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
      &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
