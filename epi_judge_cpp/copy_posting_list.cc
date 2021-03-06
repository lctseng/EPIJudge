#include "posting_list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <map>
#include <memory>
#include <unordered_map>
using std::make_shared;
using std::shared_ptr;
using std::unordered_map;

shared_ptr<PostingListNode>
CopyPostingsList(const shared_ptr<PostingListNode> &l) {
  if (!l)
    return nullptr;
  // create copied node next to me
  auto current = l;
  while (current) {
    auto next = current->next;
    auto newCurrent =
        make_shared<PostingListNode>(current->order, next, nullptr);
    current->next = newCurrent;
    current = next;
  }
  // cross updating jump
  current = l;
  while (current) {
    // BE CAREFUL!
    // current jump may not exists!
    if (current->jump) {
      current->next->jump = current->jump->next.get();
    }
    current = current->next->next;
  }
  // split lists
  auto dummyHead = make_shared<PostingListNode>(0, nullptr, nullptr);
  auto newCurrent = dummyHead;
  current = l;
  while (current) {
    newCurrent->next = current->next;
    newCurrent = newCurrent->next;
    current->next = current->next->next;
    current = current->next;
  }
  return dummyHead->next;
}

shared_ptr<PostingListNode>
CopyPostingsListHashTable(const shared_ptr<PostingListNode> &l) {
  if (!l)
    return nullptr;
  // maintain a copied new
  // map old to new
  unordered_map<PostingListNode *, shared_ptr<PostingListNode>> oldToNew;
  auto current = l;
  auto newHead = make_shared<PostingListNode>(l->order, nullptr, nullptr);
  oldToNew.insert(make_pair(l.get(), newHead));
  auto currentHead = newHead;
  auto LoadOrCreated =
      [&](PostingListNode *oldPtr) -> shared_ptr<PostingListNode> {
    if (!oldPtr)
      return nullptr;
    auto newIt = oldToNew.find(oldPtr);
    if (newIt == oldToNew.end()) {
      // create a new
      auto newNode =
          make_shared<PostingListNode>(oldPtr->order, nullptr, nullptr);
      oldToNew.insert(make_pair(oldPtr, newNode));
      return newNode;
    } else {
      return newIt->second;
    }
  };
  while (current) {
    // in this loop, assume current pointer existed
    currentHead->jump = LoadOrCreated(current->jump).get();
    currentHead->next = LoadOrCreated(current->next.get());
    currentHead = currentHead->next;
    current = current->next;
  }
  return newHead;
}
using PostingListPtr = std::shared_ptr<PostingListNode>;

struct SerializedNode {
  int order;
  int jump_index;
};

template <>
struct SerializationTraits<SerializedNode>
    : UserSerTraits<SerializedNode, int, int> {};

PostingListPtr
CreatePostingList(const std::vector<SerializedNode> &serialized) {
  std::map<int, PostingListPtr> key_mapping;
  PostingListPtr head;
  for (auto it = rbegin(serialized); it != rend(serialized); ++it) {
    head = make_shared<PostingListNode>(it->order, head, nullptr);
    key_mapping[it->order] = head;
  }
  auto list_it = head;
  for (auto it = begin(serialized); it != end(serialized);
       ++it, list_it = list_it->next) {
    if (it->jump_index != -1) {
      list_it->jump = key_mapping[it->jump_index].get();
      if (!list_it->jump)
        throw std::runtime_error("Jump index out of range");
    }
  }

  return head;
}

void AssertListsEqual(const PostingListPtr &orig, const PostingListPtr &copy) {
  std::map<PostingListNode *, PostingListNode *> node_mapping;
  auto o_it = orig;
  auto c_it = copy;
  while (o_it) {
    if (!c_it) {
      throw TestFailure("Copied list has fewer nodes than the original");
    }
    if (o_it->order != c_it->order) {
      throw TestFailure("Order value mismatch");
    }
    node_mapping[o_it.get()] = c_it.get();
    o_it = o_it->next;
    c_it = c_it->next;
  }

  if (c_it) {
    throw TestFailure("Copied list has more nodes than the original");
  }

  o_it = orig;
  c_it = copy;
  while (o_it) {
    if (node_mapping.count(c_it.get())) {
      throw TestFailure("Copied list contains a node from the original list");
    }
    if (node_mapping[o_it->jump] != c_it->jump) {
      throw TestFailure(
          "Jump link points to a different nodes in the copied list");
    }
    o_it = o_it->next;
    c_it = c_it->next;
  }
}

void CopyPostingsListWrapper(TimedExecutor &executor,
                             const std::vector<SerializedNode> &l) {
  auto head = CreatePostingList(l);

  auto copy = executor.Run([&] { return CopyPostingsList(head); });

  AssertListsEqual(head, copy);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l"};
  return GenericTestMain(args, "copy_posting_list.cc", "copy_posting_list.tsv",
                         &CopyPostingsListWrapper, DefaultComparator{},
                         param_names);
}
