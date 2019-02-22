#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> MergeTwoSortedLists(shared_ptr<ListNode<int>> L1,
                                              shared_ptr<ListNode<int>> L2) {
  if (L1 && !L2)
    return L1;
  else if (!L1 && L2)
    return L2;
  else if (!L1 && !L2)
    return nullptr;
  // now, L1 and L2 are gauranteed to be existed
  // find head
  shared_ptr<ListNode<int>> head(new ListNode<int>);
  shared_ptr<ListNode<int>> current = head;
  while (L1 && L2) {
    if (L1->data < L2->data) {
      current->next = L1;
      L1 = L1->next;
    } else {
      current->next = L2;
      L2 = L2->next;
    }
    current = current->next;
  }
  // concat remaining
  if (L1) {
    current->next = L1;
  } else if (L2) {
    current->next = L2;
  }
  return head->next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L1", "L2"};
  return GenericTestMain(args, "sorted_lists_merge.cc",
                         "sorted_lists_merge.tsv", &MergeTwoSortedLists,
                         DefaultComparator{}, param_names);
}
