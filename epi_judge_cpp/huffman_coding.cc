
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include <memory>
#include <queue>
#include <string>
#include <vector>
using std::greater;
using std::make_shared;
using std::priority_queue;
using std::shared_ptr;
using std::string;
using std::vector;
namespace huffman {

template <typename T> struct BinaryTreeNode {
  T data;
  shared_ptr<BinaryTreeNode<T>> left, right;

  explicit BinaryTreeNode(const T &data) : data(data) {}
  BinaryTreeNode(T data, shared_ptr<BinaryTreeNode<T>> left,
                 shared_ptr<BinaryTreeNode<T>> right)
      : data(data), left(left), right(right) {}
};

struct CharWithFrequency {
  char c;
  double freq;
};

typedef BinaryTreeNode<CharWithFrequency> TreeNode;

bool operator>(shared_ptr<TreeNode> a, shared_ptr<TreeNode> b) {
  return a->data.freq > b->data.freq;
}

double ComputePathSum(shared_ptr<TreeNode> current, int depth) {
  double val = 0;
  if (current->left && current->right) {
    val += ComputePathSum(current->left, depth + 1);
    val += ComputePathSum(current->right, depth + 1);
  } else {
    // leaf node
    val += current->data.freq * depth;
  }
  return val;
}

double HuffmanEncoding(vector<CharWithFrequency> *symbols) {
  if (symbols->empty())
    return 0;
  priority_queue<shared_ptr<TreeNode>, vector<shared_ptr<TreeNode>>,
                 greater<shared_ptr<TreeNode>>>
      pq;
  for (auto &val : *symbols) {
    pq.push(make_shared<TreeNode>(val));
  }
  while (pq.size() > 1) {
    auto node1 = pq.top();
    pq.pop();
    auto node2 = pq.top();
    pq.pop();
    // form a subtree
    auto next = make_shared<TreeNode>(node1->data);
    next->left = node1;
    next->right = node2;
    next->data.freq += node2->data.freq;
    pq.push(next);
  }
  return ComputePathSum(pq.top(), 0) / 100.0;
}
} // namespace huffman
template <>
struct SerializationTraits<huffman::CharWithFrequency>
    : UserSerTraits<huffman::CharWithFrequency, std::string, double> {
  static huffman::CharWithFrequency
  FromTuple(const std::tuple<std::string, double> &values) {
    if (std::get<0>(values).size() != 1) {
      throw std::runtime_error(
          "CharWithFrequency parser: string length is not 1");
    }
    return huffman::CharWithFrequency{std::get<0>(values)[0],
                                      std::get<1>(values)};
  }
};

double HuffmanEncodingWrapper(vector<huffman::CharWithFrequency> symbols) {
  return huffman::HuffmanEncoding(&symbols);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"symbols"};
  return GenericTestMain(args, "huffman_coding.cc", "huffman_coding.tsv",
                         &HuffmanEncodingWrapper, DefaultComparator{},
                         param_names);
}
