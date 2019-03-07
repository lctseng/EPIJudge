#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
#include <stdexcept>
#include <vector>
using std::vector;

struct GraphVertex {
  vector<GraphVertex *> edges;
  bool visited = false;
};

bool dfs(GraphVertex *current) {
  if (!current)
    return false;
  current->visited = true;
  // explore me
  for (auto *nextNode : current->edges) {
    if (nextNode->visited || dfs(nextNode))
      return true;
  }
  return false;
}

bool IsDeadlocked(vector<GraphVertex> *graph) {
  // is cycle exists?
  // for every start point
  // BE CAREFUL!
  // dont need to reset 'seen' every time
  // we need a O(n) not O(n^2)!!!
  for (auto &node : *graph) {
    if (!node.visited && dfs(&node))
      return true;
  }
  return false;
}
struct Edge {
  int from;
  int to;
};

template <> struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

bool HasCycleWrapper(TimedExecutor &executor, int num_nodes,
                     const vector<Edge> &edges) {
  vector<GraphVertex> graph;
  if (num_nodes <= 0) {
    throw std::runtime_error("Invalid num_nodes value");
  }
  graph.reserve(num_nodes);

  for (int i = 0; i < num_nodes; i++) {
    graph.push_back(GraphVertex{});
  }

  for (const Edge &e : edges) {
    if (e.from < 0 || e.from >= num_nodes || e.to < 0 || e.to >= num_nodes) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return IsDeadlocked(&graph); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "num_nodes", "edges"};
  return GenericTestMain(args, "deadlock_detection.cc",
                         "deadlock_detection.tsv", &HasCycleWrapper,
                         DefaultComparator{}, param_names);
}
