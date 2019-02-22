#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> EvenOddMerge(shared_ptr<ListNode<int>> L) {
  if (!L)
    return nullptr;
  auto evenHead = L;
  L = L->next;
  if (!L)
    return evenHead;
  auto oddHead = L;
  L = L->next;
  bool isEven = true;
  auto evenCur = evenHead;
  auto oddCur = oddHead;
  while (L) {
    if (isEven) {
      evenCur->next = L;
      L = L->next;
      evenCur = evenCur->next;
    } else {
      oddCur->next = L;
      L = L->next;
      oddCur = oddCur->next;
    }
    isEven = !isEven;
  }
  // merge
  evenCur->next = oddHead;
  oddCur->next = nullptr;
  return evenHead;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "even_odd_list_merge.cc",
                         "even_odd_list_merge.tsv", &EvenOddMerge,
                         DefaultComparator{}, param_names);
}
