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
    int mid;
    for (mid = begin + 1; mid < end; mid++) {
      if (preorder_sequence[mid] > preorder_sequence[begin])
        break;
    }
    root->left =
        RebuildBSTFromPreorderHelper(preorder_sequence, begin + 1, mid);
    root->right = RebuildBSTFromPreorderHelper(preorder_sequence, mid, end);
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
