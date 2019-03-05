#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <memory>
#include <vector>
using std::make_unique;
using std::move;
using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>>
BuildMinHeightBSTFromSortedArrayHelper(const vector<int> &A, int begin,
                                       int end) {
  if (begin >= end) {
    return nullptr;
  } else {
    int mid = begin + (end - begin) / 2;
    auto root = make_unique<BstNode<int>>(A[mid]);
    root->left = BuildMinHeightBSTFromSortedArrayHelper(A, begin, mid);
    root->right = BuildMinHeightBSTFromSortedArrayHelper(A, mid + 1, end);
    return move(root);
  }
}

unique_ptr<BstNode<int>>
BuildMinHeightBSTFromSortedArray(const vector<int> &A) {
  return BuildMinHeightBSTFromSortedArrayHelper(A, 0, A.size());
}
int BuildMinHeightBSTFromSortedArrayWrapper(TimedExecutor &executor,
                                            const vector<int> &A) {
  unique_ptr<BstNode<int>> result =
      executor.Run([&] { return BuildMinHeightBSTFromSortedArray(A); });

  if (GenerateInorder(result) != A) {
    throw TestFailure("Result binary tree mismatches input array");
  }
  return BinaryTreeHeight(result);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A"};
  return GenericTestMain(args, "bst_from_sorted_array.cc",
                         "bst_from_sorted_array.tsv",
                         &BuildMinHeightBSTFromSortedArrayWrapper,
                         DefaultComparator{}, param_names);
}
