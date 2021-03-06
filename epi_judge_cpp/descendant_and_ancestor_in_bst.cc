#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <memory>
using std::max;
using std::unique_ptr;

bool PairIncludesAncestorAndDescendantOfM(
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_1,
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_2,
    const unique_ptr<BstNode<int>> &middle) {
  if (!middle || !possible_anc_or_desc_1 || !possible_anc_or_desc_2)
    return false;
  // algo: interleaving walk down from node1 and node2
  // currentA: 1->2
  // currentB: 2->1
  BstNode<int> *currentA = possible_anc_or_desc_1.get();
  BstNode<int> *currentB = possible_anc_or_desc_2.get();
  // until they meet middle or meet null/the other node
  while ((currentA && currentA != middle.get() &&
          currentA != possible_anc_or_desc_2.get()) ||
         (currentB && currentB != middle.get() &&
          currentB != possible_anc_or_desc_1.get())) {
    if (currentA && currentA != middle.get() &&
        currentA != possible_anc_or_desc_2.get()) {
      // go down A
      if (middle->data < currentA->data) {
        currentA = currentA->left.get();
      } else {
        currentA = currentA->right.get();
      }
    }
    if (currentB && currentB != middle.get() &&
        currentB != possible_anc_or_desc_1.get()) {
      // go down B
      if (middle->data < currentB->data) {
        currentB = currentB->left.get();
      } else {
        currentB = currentB->right.get();
      }
    }
  }
  // now, A and B stopped, check why they stop
  // let go to the other same as go to null
  if (currentA == possible_anc_or_desc_2.get())
    currentA = nullptr;
  if (currentB == possible_anc_or_desc_1.get())
    currentB = nullptr;
  // if both meet null/the other, return false
  if (!currentA && !currentB)
    return false;
  // now, non-null pointer is middle, go to find the other end
  // for nodes do meet middle, go from middle to see whether we can meet target
  while (currentA || currentB) {
    if (currentA) {
      if (currentA->data == possible_anc_or_desc_2->data) {
        return true;
      } else if (currentA->data > possible_anc_or_desc_2->data) {
        currentA = currentA->left.get();
      } else {
        currentA = currentA->right.get();
      }
    }
    if (currentB) {
      if (currentB->data == possible_anc_or_desc_1->data) {
        return true;
      } else if (currentB->data > possible_anc_or_desc_1->data) {
        currentB = currentB->left.get();
      } else {
        currentB = currentB->right.get();
      }
    }
  }
  // still not found
  return false;
}

// int totalTraverseCount;
// int maxSingleCount;

// enum Stage { FIND_MID, FIND_DESC };

// bool PairIncludesAncestorAndDescendantOfM(
//     const unique_ptr<BstNode<int>> &possible_anc_or_desc_0,
//     const unique_ptr<BstNode<int>> &possible_anc_or_desc_1,
//     const unique_ptr<BstNode<int>> &middle) {
//   // perform interleaving!
//   BstNode<int> *currentA = possible_anc_or_desc_0.get();
//   BstNode<int> *currentB = possible_anc_or_desc_1.get();
//   Stage stageA = FIND_MID, stageB = FIND_MID;
//   int currentCount = 0;
//   while (currentA || currentB) {
//     if (currentA) {
//       BstNode<int> *target =
//           stageA == FIND_MID ? middle.get() : possible_anc_or_desc_1.get();
//       if (currentA->data > target->data) {
//         ++currentCount;
//         currentA = currentA->left.get();
//       } else if (currentA->data < target->data) {
//         ++currentCount;
//         currentA = currentA->right.get();
//       } else {
//         // found target
//         if (stageA == FIND_MID) {
//           // go advance to find the desc
//           stageA = FIND_DESC;
//         } else {
//           // we are in find find desc and we found the target
//           // note that if desc == middle, is false
//           if (middle.get() != target) {
//             totalTraverseCount += currentCount;
//             maxSingleCount = max(maxSingleCount, currentCount);
//             // printf("Current: %d\n", currentCount);
//             return true;
//           } else {
//             // A failed
//             currentA = nullptr;
//           }
//         }
//       }
//     }
//     if (currentB) {
//       BstNode<int> *target =
//           stageB == FIND_MID ? middle.get() : possible_anc_or_desc_0.get();
//       if (currentB->data > target->data) {
//         ++currentCount;
//         currentB = currentB->left.get();
//       } else if (currentB->data < target->data) {
//         ++currentCount;
//         currentB = currentB->right.get();
//       } else {
//         // found target
//         if (stageB == FIND_MID) {
//           // go advance to find the desc
//           stageB = FIND_DESC;
//         } else {
//           // we are in find find desc and we found the target
//           // note that if desc == middle, is false
//           if (middle.get() != target) {
//             totalTraverseCount += currentCount;
//             maxSingleCount = max(maxSingleCount, currentCount);
//             // printf("Current: %d\n", currentCount);
//             return true;
//           } else {
//             // B failed
//             currentB = nullptr;
//           }
//         }
//       }
//     }
//   }
//   totalTraverseCount += currentCount;
//   maxSingleCount = max(maxSingleCount, currentCount);
//   // printf("Current: %d\n", currentCount);
//   return false;
// }

// int slowCurrentCount;
// int IsPossibleAncestorOf(const unique_ptr<BstNode<int>> &ancestor,
//                          const unique_ptr<BstNode<int>> &target) {
//   // search until reach the null
//   BstNode<int> *current = ancestor.get();
//   int distance = 0;
//   while (current) {
//     if (current->data > target->data) {
//       ++slowCurrentCount;
//       current = current->left.get();
//     } else if (current->data < target->data) {
//       ++slowCurrentCount;
//       current = current->right.get();
//     } else {
//       return distance;
//     }
//     ++distance;
//   }
//   // not found
//   return -1;
// }

// bool PairIncludesAncestorAndDescendantOfMSlow(
//     const unique_ptr<BstNode<int>> &possible_anc_or_desc_0,
//     const unique_ptr<BstNode<int>> &possible_anc_or_desc_1,
//     const unique_ptr<BstNode<int>> &middle) {
//   slowCurrentCount = 0;
//   // brute force try :P
//   // is 0->M->1?
//   if (IsPossibleAncestorOf(possible_anc_or_desc_0, middle) >= 0 &&
//       IsPossibleAncestorOf(middle, possible_anc_or_desc_1) >= 1) {
//     totalTraverseCount += slowCurrentCount;
//     maxSingleCount = max(maxSingleCount, slowCurrentCount);
//     // printf("Current: %d\n", slowCurrentCount);
//     return true;
//   }

//   // is 1->M->0?
//   if (IsPossibleAncestorOf(possible_anc_or_desc_1, middle) >= 0 &&
//       IsPossibleAncestorOf(middle, possible_anc_or_desc_0) >= 1) {
//     totalTraverseCount += slowCurrentCount;
//     maxSingleCount = max(maxSingleCount, slowCurrentCount);
//     // printf("Current: %d\n", slowCurrentCount);
//     return true;
//   }
//   // otherwise
//   totalTraverseCount += slowCurrentCount;
//   maxSingleCount = max(maxSingleCount, slowCurrentCount);
//   // printf("Current: %d\n", slowCurrentCount);
//   return false;
// }
bool PairIncludesAncestorAndDescendantOfMWrapper(
    TimedExecutor &executor, const unique_ptr<BstNode<int>> &tree,
    int possible_anc_or_desc_0, int possible_anc_or_desc_1, int middle) {
  auto &candidate0 = MustFindNode(tree, possible_anc_or_desc_0);
  auto &candidate1 = MustFindNode(tree, possible_anc_or_desc_1);
  auto &middle_node = MustFindNode(tree, middle);
  return executor.Run([&] {
    return PairIncludesAncestorAndDescendantOfM(candidate0, candidate1,
                                                middle_node);
  });
}

int main(int argc, char *argv[]) {
  // totalTraverseCount = 0;
  // maxSingleCount = 0;
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree",
                                       "possible_anc_or_desc_0",
                                       "possible_anc_or_desc_1", "middle"};
  auto ret = GenericTestMain(args, "descendant_and_ancestor_in_bst.cc",
                             "descendant_and_ancestor_in_bst.tsv",
                             &PairIncludesAncestorAndDescendantOfMWrapper,
                             DefaultComparator{}, param_names);
  // printf("Total traverse count: %d\n", totalTraverseCount);
  // printf("Max single traverse count: %d\n", maxSingleCount);
  return ret;
}
