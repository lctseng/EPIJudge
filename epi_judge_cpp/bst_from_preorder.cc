#include "bst_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::move;
using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>>
RebuildBSTFromPreorderHelper(const vector<int> &preorder_sequence, int begin,
                             int end) {
  if (begin >= end) {
    return nullptr;
  } else {
    unique_ptr<BstNode<int>> root{new BstNode<int>(preorder_sequence[begin])};
    // find split point
    // using binary search
    int pivot = -1;
    int low = begin + 1, high = end;
    while (low < high) {
      int mid = low + (high - low) / 2;
      if (preorder_sequence[mid] > preorder_sequence[begin] &&
          (mid == begin + 1 ||
           preorder_sequence[mid - 1] < preorder_sequence[begin])) {
        pivot = mid;
        // BE CAREFUL : binary search terminate needs exit loop
        break;
      } else if (preorder_sequence[mid] < preorder_sequence[begin]) {
        // go right
        low = mid + 1;
      } else {
        // go left
        high = mid;
      }
    }
    // BE CAREFUL : binary search out-of-bound
    if (pivot < 0)
      pivot = low;

    // build child
    root->left =
        RebuildBSTFromPreorderHelper(preorder_sequence, begin + 1, pivot);
    root->right = RebuildBSTFromPreorderHelper(preorder_sequence, pivot, end);
    return move(root);
  }
}

unique_ptr<BstNode<int>>
RebuildBSTFromPreorder(const vector<int> &preorder_sequence) {
  return RebuildBSTFromPreorderHelper(preorder_sequence, 0,
                                      preorder_sequence.size());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder_sequence"};
  return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                         &RebuildBSTFromPreorder, DefaultComparator{},
                         param_names);
}
