#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <set>
#include <stdexcept>
using std::make_shared;

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

shared_ptr<ListNode<int>> CheckListHasCycle(shared_ptr<ListNode<int>> head) {
  // use dummy head
  shared_ptr<ListNode<int>> dummy = make_shared<ListNode<int>>(0, head);
  auto slow = dummy;
  auto fast = dummy;
  auto finder = dummy;
  while (true) {
    slow = slow->next;
    fast = fast->next;
    if (!fast)
      return nullptr;
    fast = fast->next;
    if (!fast)
      return nullptr;
    if (slow == fast)
      break;
  }
  while (true) {
    slow = slow->next;
    finder = finder->next;
    if (slow == finder) {
      return slow;
    }
  }
}

int Distance(shared_ptr<ListNode<int>> a, shared_ptr<ListNode<int>> b) {
  int d = 0;
  while (a != b) {
    a = a->next;
    ++d;
  }
  return d;
}

shared_ptr<ListNode<int>> OverlappingLists(shared_ptr<ListNode<int>> l0,
                                           shared_ptr<ListNode<int>> l1) {
  // check has cycle?
  auto cycle0 = CheckListHasCycle(l0);
  auto cycle1 = CheckListHasCycle(l1);
  if (!cycle0 && !cycle1) {
    // both has no cycle
    return OverlappingNoCycleLists(l0, l1);
  }
  if ((!cycle0 && cycle1) || (cycle1 && !cycle0)) {
    // one has cycle, the other does not
    return nullptr;
  }
  // both has cycle
  // check the cycle is same cycle?
  // can we start from one cycle and reach the other?
  auto temp = cycle0->next;
  while (temp != cycle0 && temp != cycle1) {
    temp = temp->next;
  }
  if (temp != cycle1)
    return nullptr;
  // now two list has the same cycle in the end
  // two case:
  // case1:  overlapped before cycle start
  int distance0 = Distance(l0, cycle0);
  int distance1 = Distance(l1, cycle1);
  // advance together
  if (distance0 > distance1) {
    l0 = advanceByK(l0, distance0 - distance1);
  } else if (distance1 > distance0) {
    l1 = advanceByK(l1, distance1 - distance0);
  }
  while (l0 != cycle0 && l1 != cycle1 && l0 != l1) {
    l0 = l0->next;
    l1 = l1->next;
  }
  if (l0 == l1) {
    // l0 meet l1 before the cycle
    return l0;
  } else {
    // case2: overlapped in the cycle. this time both cycle0 and cycle1 are
    // acceptable
    return cycle0; // or cycle1
  }
}

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
OverlappingListsModified(shared_ptr<ListNode<int>> l0,
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
