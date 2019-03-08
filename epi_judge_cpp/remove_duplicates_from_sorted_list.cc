#include "list_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::shared_ptr;

shared_ptr<ListNode<int>> RemoveDuplicates(const shared_ptr<ListNode<int>> &L) {
  if (!L)
    return L;
  auto current = L;
  while (current) {
    if (current->next && current->data == current->next->data) {
      current->next = current->next->next;
    } else {
      // BE CAREFUL
      // only advance when different
      current = current->next;
    }
  }
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "remove_duplicates_from_sorted_list.cc",
                         "remove_duplicates_from_sorted_list.tsv",
                         &RemoveDuplicates, DefaultComparator{}, param_names);
}
