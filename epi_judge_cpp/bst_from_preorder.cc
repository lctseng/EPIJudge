#include "bst_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::move;
using std::unique_ptr;
using std::vector;

// parentIndex: cannot consume value larger than that with parentIndex
unique_ptr<BstNode<int>>
RebuildBSTFromPreorderHelper(const vector<int> &preorder_sequence,
                             int parentIndex, int &index) {
  if (index >= preorder_sequence.size() ||
      (parentIndex >= 0 &&
       preorder_sequence[parentIndex] < preorder_sequence[index])) {
    return nullptr;
  } else {
    int currentIndex = index++;
    unique_ptr<BstNode<int>> root{
        new BstNode<int>(preorder_sequence[currentIndex])};
    root->left =
        RebuildBSTFromPreorderHelper(preorder_sequence, currentIndex, index);
    root->right =
        RebuildBSTFromPreorderHelper(preorder_sequence, parentIndex, index);
    return move(root);
  }
}

unique_ptr<BstNode<int>>
RebuildBSTFromPreorder(const vector<int> &preorder_sequence) {
  int nextIndex = 0;
  return RebuildBSTFromPreorderHelper(preorder_sequence, -1, nextIndex);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder_sequence"};
  return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                         &RebuildBSTFromPreorder, DefaultComparator{},
                         param_names);
}
