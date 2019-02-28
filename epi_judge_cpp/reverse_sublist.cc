#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {
  if (!L || start == finish)
    return L;
  auto dummy = make_shared<ListNode<int>>(0, L);
  // find the first to be reversed
  auto reversedTail = L, preTail = dummy;
  for (int i = 1; i < start; i++) {
    preTail = reversedTail;
    reversedTail = reversedTail->next;
  }
  auto prev = reversedTail, current = reversedTail->next;
  for (int i = start; i < finish; i++) {
    auto next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  // after that, prev points to new head
  // current points to past-over-1 next
  // fix it!
  preTail->next = prev;
  reversedTail->next = current;
  return dummy->next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
