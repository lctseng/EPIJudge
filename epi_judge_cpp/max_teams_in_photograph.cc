#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
#include <stdexcept>
#include <vector>
using std::max;
using std::vector;
struct GraphVertex {
  vector<GraphVertex *> edges;
  // Set max_distance = 0 to indicate unvisited vertex.
  int max_distance = 0;
};

int dfs(GraphVertex *current) {
  // for every edge, find max
  int maxD = 0;
  for (auto *nextNode : current->edges) {
    maxD = max(maxD,
               nextNode->max_distance ? nextNode->max_distance : dfs(nextNode));
  }
  return current->max_distance = maxD + 1;
}

int FindLargestNumberTeams(vector<GraphVertex> *graph) {
  // dfs every node, find the max length
  // tsort, when returning, record the max distance behind
  // distance recorded in the node, advance if we can update the
  int max_distance = 0;
  for (auto &node : *graph) {
    if (node.max_distance == 0) {
      max_distance = max(max_distance, dfs(&node));
    }
    // if already has distance, it is never the max distance
  }
  return max_distance;
}
struct Edge {
  int from;
  int to;
};

template <> struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

int FindLargestNumberTeamsWrapper(TimedExecutor &executor, int k,
                                  const vector<Edge> &edges) {
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }

  vector<GraphVertex> graph(k, GraphVertex{});

  for (const Edge &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return FindLargestNumberTeams(&graph); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "edges"};
  return GenericTestMain(
      args, "max_teams_in_photograph.cc", "max_teams_in_photograph.tsv",
      &FindLargestNumberTeamsWrapper, DefaultComparator{}, param_names);
}
