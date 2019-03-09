#include "list_node.h"
#include "test_framework/generic_test.h"
using std::min;
int Length(shared_ptr<ListNode<int>> l) {
  int r = 0;
  while (l) {
    ++r;
    l = l->next;
  }
  return r;
}

shared_ptr<ListNode<int>> AddTwoNumbers(shared_ptr<ListNode<int>> L1,
                                        shared_ptr<ListNode<int>> L2) {
  // easy sol: short one add to long one
  int len1 = Length(L1), len2 = Length(L2);
  shared_ptr<ListNode<int>> base = nullptr, addend = nullptr;
  if (len1 >= len2) {
    base = L1;
    addend = L2;
  } else {
    base = L2;
    addend = L1;
  }
  // stage 1: add common part, with carry
  int minLen = min(len1, len2);
  auto baseCurrent = base, addendCurrent = addend;
  shared_ptr<ListNode<int>> basePrev = nullptr;
  int carry = 0;
  while (minLen-- > 0) {
    baseCurrent->data += carry + addendCurrent->data;
    if (baseCurrent->data >= 10) {
      baseCurrent->data -= 10;
      carry = 1;
    } else {
      // we have no carry for next digit
      carry = 0;
    }
    basePrev = baseCurrent;
    baseCurrent = baseCurrent->next;
    addendCurrent = addendCurrent->next;
  }
  // stage 2: end of addend, continue add with carry with base
  while (baseCurrent) {
    baseCurrent->data += carry;
    if (baseCurrent->data >= 10) {
      baseCurrent->data -= 10;
      carry = 1;
    } else {
      carry = 0;
    }
    basePrev = baseCurrent;
    baseCurrent = baseCurrent->next;
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
