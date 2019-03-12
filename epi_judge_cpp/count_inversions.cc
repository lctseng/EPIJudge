#include "test_framework/generic_test.h"
#include <memory>
#include <vector>
using std::make_shared;
using std::shared_ptr;
using std::stable_sort;
using std::vector;

struct BstNode {
  shared_ptr<BstNode> left, right;
  int count;
  int value;
  BstNode(int value, int count = 1, shared_ptr<BstNode> left = nullptr,
          shared_ptr<BstNode> right = nullptr)
      : value(value), count(count), left(left), right(right) {}
};

class Bst {
public:
  void insert(int val) {
    if (!root) {
      root = make_shared<BstNode>(val);
    } else {
      auto current = root;
      shared_ptr<BstNode> prev = nullptr;
      while (current) {
        current->count++;
        if (val > current->value) {
          // insert right
          prev = current;
          current = current->right;
        } else {
          // insert left
          prev = current;
          current = current->left;
        }
        // note: there is no equal
      }
      // found position
      if (prev->value > val) {
        // at left
        prev->left = make_shared<BstNode>(val);
      } else {
        // at right
        prev->right = make_shared<BstNode>(val);
      }
    }
  }

  int countLargerThan(int val) {
    int count = 0;
    auto current = root;
    while (current) {
      if (val > current->value) {
        // go right directly
        current = current->right;
      } else {
        // go left and record right value
        // val < current->value
        int rightCount = current->right ? current->right->count : 0;
        count += 1 + rightCount;
        current = current->left;
      }
    }
    return count;
  }

private:
  shared_ptr<BstNode> root = nullptr;
};

struct Entry {
  int value;
  int index;
};

int CountInversions(vector<int> A) {
  vector<Entry> data;
  for (int i = 0; i < A.size(); i++) {
    data.push_back({A[i], i});
  }
  // sort data according to value
  // stable sort is REQUIRED!
  // for handle same number, need to treat it as ALREADY IN ORDER
  stable_sort(data.begin(), data.end(),
              [](const Entry &a, const Entry &b) { return a.value < b.value; });
  // process each data
  Bst tree;
  int count = 0;
  for (int i = 0; i < data.size(); i++) {
    auto &ent = data[i];
    // query how many number has larger index?
    count += tree.countLargerThan(ent.index);
    // beware of same number!
    tree.insert(ent.index);
  }
  return count;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "count_inversions.cc", "count_inversions.tsv",
                         &CountInversions, DefaultComparator{}, param_names);
}
