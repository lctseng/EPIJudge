#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {
  if (!L || start == finish)
    return L;
  // find: before, first, last, past
  shared_ptr<ListNode<int>> before = nullptr, first = L, last = nullptr,
                            past = nullptr;
  for (int i = 1; i < start; i++) {
    before = first;
    first = first->next;
  }
  last = first;
  past = first->next;
  for (int i = start; i < finish; i++) {
    last = past;
    past = past->next;
  }
  // reverse between first and last
  auto prev = first, current = first->next;
  while (true) {
    auto next = current->next;
    current->next = prev;
    // check last one?
    if (current == last)
      break;
    prev = current;
    current = next;
  }
  // fix before and first's next
  if (before)
    before->next = last;
  first->next = past;
  // if the start is 1, we return new node (last)
  // otherwise we return L
  return start == 1 ? last : L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
