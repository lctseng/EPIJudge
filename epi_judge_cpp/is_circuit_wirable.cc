#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
#include <stdexcept>
#include <vector>
using std::vector;

#define flipColor(i) (-i + 1)
const int kWhite = 1;
const int kBlack = 0;
const int kUnknown = -1;

struct GraphVertex {
  int d = -1;
  vector<GraphVertex *> edges;
};

bool ColorNode(GraphVertex *current) {
  if (!current)
    return true;
  // explore its edges
  int revColor = flipColor(current->d);
  for (auto *nextNode : current->edges) {
    if (nextNode->d == kUnknown) {
      // color it
      nextNode->d = revColor;
      if (!ColorNode(nextNode))
        return false;
    } else if (nextNode->d == current->d) {
      return false;
    }
    // otherwise, meet a colored revColor, just skip it
  }
  return true;
}

bool IsAnyPlacementFeasible(vector<GraphVertex> *graph) {
  // dfs each node
  // process every d = -1 node, color with white
  // return false if found contradiction
  for (auto &node : *graph) {
    if (node.d == kUnknown) {
      node.d = kWhite;
      if (!ColorNode(&node))
        return false;
    }
  }
  return true;
}
struct Edge {
  int from;
  int to;
};

template <> struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

bool IsAnyPlacementFeasibleWrapper(TimedExecutor &executor, int k,
                                   const vector<Edge> &edges) {
  vector<GraphVertex> graph;
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }
  graph.reserve(k);

  for (int i = 0; i < k; i++) {
    graph.push_back(GraphVertex{});
  }

  for (auto &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return IsAnyPlacementFeasible(&graph); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "edges"};
  return GenericTestMain(
      args, "is_circuit_wirable.cc", "is_circuit_wirable.tsv",
      &IsAnyPlacementFeasibleWrapper, DefaultComparator{}, param_names);
}
