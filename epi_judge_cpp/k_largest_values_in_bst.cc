#include "bst_node.h"
#include "test_framework/generic_test.h"
#include <memory>
#include <vector>
using std::unique_ptr;
using std::vector;

vector<int> res;

void FindKLargestInBSTHelper(const unique_ptr<BstNode<int>> &tree, int &k) {
  if (!tree || k <= 0)
    return;
  FindKLargestInBSTHelper(tree->right, k);
  if (k <= 0)
    return;
  res.push_back(tree->data);
  k--;
  if (k <= 0)
    return;
  FindKLargestInBSTHelper(tree->left, k);
}

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>> &tree, int k) {
  res.clear();
  FindKLargestInBSTHelper(tree, k);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "k"};
  return GenericTestMain(args, "k_largest_values_in_bst.cc",
                         "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
