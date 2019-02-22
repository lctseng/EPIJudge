#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <set>
#include <stdexcept>

shared_ptr<ListNode<int>>
FindCycleInListPrev(shared_ptr<ListNode<int>> origHead) {
  if (!origHead)
    return nullptr;
  shared_ptr<ListNode<int>> head(new ListNode<int>({}, origHead));
  auto slow = head, fast = head, prevFast = head;
  while (true) {
    slow = slow->next;
    prevFast = fast;
    fast = fast->next;
    if (!fast)
      return prevFast;
    prevFast = fast;
    fast = fast->next;
    if (!fast)
      return prevFast;
    if (slow == fast)
      break;
  }
  auto finder = head, prevSlow = head;
  while (true) {
    prevSlow = slow;
    slow = slow->next;
    finder = finder->next;
    if (slow == finder) {
      return prevSlow;
    }
  }
}

shared_ptr<ListNode<int>>
OverlappingNoCycleLists(shared_ptr<ListNode<int>> l0,
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
}

shared_ptr<ListNode<int>> OverlappingLists(shared_ptr<ListNode<int>> l0,
                                           shared_ptr<ListNode<int>> l1) {
  // edge case: one list is empty
  if (!l0 || !l1)
    return nullptr;
  // edge case: fully overlapped
  if (l0 == l1)
    return l0;
  // record and remove both cycle
  auto cycle0Prev = FindCycleInListPrev(l0);
  auto restoreNext0 = cycle0Prev->next;
  cycle0Prev->next = nullptr;
  // THE ORDER IS IMPORTANT! Since the tail may be shared :P
  auto cycle1Prev = FindCycleInListPrev(l1);
  auto restoreNext1 = cycle1Prev->next;
  cycle1Prev->next = nullptr;
  // perform non-cycle test
  auto res = OverlappingNoCycleLists(l0, l1);

  // restore cycle if needed
  cycle0Prev->next = restoreNext0;
  cycle1Prev->next = restoreNext1;
  return res;
}
void OverlappingListsWrapper(TimedExecutor &executor,
                             shared_ptr<ListNode<int>> l0,
                             shared_ptr<ListNode<int>> l1,
                             shared_ptr<ListNode<int>> common, int cycle0,
                             int cycle1) {
  if (common) {
    if (!l0) {
      l0 = common;
    } else {
      auto it = l0;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }

    if (!l1) {
      l1 = common;
    } else {
      auto it = l1;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }
  }

  if (cycle0 != -1 && l0) {
    auto last = l0;
    while (last->next) {
      last = last->next;
    }
    auto it = l0;
    while (cycle0-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  if (cycle1 != -1 && l1) {
    auto last = l1;
    while (last->next) {
      last = last->next;
    }
    auto it = l1;
    while (cycle1-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  std::set<shared_ptr<ListNode<int>>> common_nodes;
  auto it = common;
  while (it && common_nodes.count(it) == 0) {
    common_nodes.insert(it);
    it = it->next;
  }

  auto result = executor.Run([&] { return OverlappingLists(l0, l1); });

  if (!((common_nodes.empty() && result == nullptr) ||
        common_nodes.count(result) > 0)) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0",     "l1",
                                       "common",   "cycle0", "cycle1"};
  return GenericTestMain(args, "do_lists_overlap.cc", "do_lists_overlap.tsv",
                         &OverlappingListsWrapper, DefaultComparator{},
                         param_names);
}
