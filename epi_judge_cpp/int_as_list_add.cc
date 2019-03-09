#include "list_node.h"
#include "test_framework/generic_test.h"
using std::min;
shared_ptr<ListNode<int>> AddTwoNumbers(shared_ptr<ListNode<int>> base,
                                        shared_ptr<ListNode<int>> addend) {
  // dirty solution: reuse and overwrite original space
  // time and space efficient!
  auto baseCurrent = base, addendCurrent = addend;
  shared_ptr<ListNode<int>> basePrev = nullptr;
  int carry = 0;
  // optimze: early terminate when no carry and next number to add
  while (baseCurrent && (carry || addendCurrent)) {
    baseCurrent->data += carry + (addendCurrent ? addendCurrent->data : 0);
    if (baseCurrent->data >= 10) {
      baseCurrent->data -= 10;
      carry = 1;
    } else {
      // we have no carry for next digit
      carry = 0;
    }
    // handle the case when len(base) < len(addend)
    if (!baseCurrent->next && addendCurrent && addendCurrent->next) {
      // next base is empty, but not addend
      // force concat base
      baseCurrent->next = addendCurrent->next;
      addendCurrent->next = nullptr;
    }
    basePrev = baseCurrent;
    baseCurrent = baseCurrent->next;
    if (addendCurrent)
      addendCurrent = addendCurrent->next;
  }

  if (carry) {
    // carry left. need append new node
    basePrev->next = make_shared<ListNode<int>>(carry);
  }
  // use base as result
  return base;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L1", "L2"};
  return GenericTestMain(args, "int_as_list_add.cc", "int_as_list_add.tsv",
                         &AddTwoNumbers, DefaultComparator{}, param_names);
}
