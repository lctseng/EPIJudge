#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::queue;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct GraphVertex {
  int label;
  vector<GraphVertex *> edges;
};

unordered_map<int, GraphVertex *> clonedNodes;

// clone the given node. read from cache if possible
GraphVertex *CloneGraphHelper(GraphVertex *graph) {
  auto clonedIt = clonedNodes.find(graph->label);
  if (clonedIt != clonedNodes.end()) {
    return clonedIt->second;
  }
  // clone this one
  auto *newNode = new GraphVertex{graph->label};
  clonedNodes[graph->label] = newNode;
  // clone each edge
  for (auto *nextNode : graph->edges) {
    newNode->edges.push_back(CloneGraphHelper(nextNode));
  }
  return newNode;
}

GraphVertex *CloneGraphDFS(GraphVertex *graph) {
  clonedNodes.clear();
  return CloneGraphHelper(graph);
}

GraphVertex *CloneGraphBFS(GraphVertex *graph) {
  clonedNodes.clear();
  queue<GraphVertex *> todos;
  auto *newGraph = new GraphVertex{graph->label};
  clonedNodes[graph->label] = newGraph;
  todos.push(graph);
  while (todos.size()) {
    auto *currentOld = todos.front();
    auto *currrentNew = clonedNodes[currentOld->label];
    todos.pop();
    // clone  edge
    for (auto *oldNextNode : currentOld->edges) {
      GraphVertex *clonedNext = nullptr;
      auto it = clonedNodes.find(oldNextNode->label);
      if (it == clonedNodes.end()) {
        clonedNext = new GraphVertex{oldNextNode->label};
        // BE CAREFUL: update the node map, so that when pop from queue, we
        // always have mapped new nodes
        clonedNodes[oldNextNode->label] = clonedNext;
        todos.push(oldNextNode);
      } else {
        clonedNext = it->second;
      }
      currrentNew->edges.push_back(clonedNext);
    }
  }
  return newGraph;
}

vector<int> CopyLabels(const vector<GraphVertex *> &edges) {
  vector<int> labels;
  transform(begin(edges), end(edges), back_inserter(labels),
            [](const auto &e) { return e->label; });
  return labels;
}

void CheckGraph(GraphVertex *node, const vector<GraphVertex> &graph) {
  if (!node || node == &graph[0]) {
    throw TestFailure("Graph was not copied");
  }

  unordered_set<GraphVertex *> vertex_set;
  queue<GraphVertex *> q;
  q.emplace(node);
  vertex_set.emplace(node);
  while (!q.empty()) {
    auto vertex = q.front();
    q.pop();
    if (vertex->label >= graph.size()) {
      throw TestFailure("Invalid vertex label");
    }
    vector<int> label1 = CopyLabels(vertex->edges),
                label2 = CopyLabels(graph[vertex->label].edges);
    sort(begin(label1), end(label1));
    sort(begin(label2), end(label2));
    if (label1 != label2) {
      throw TestFailure("Edges mismatch");
    }
    for (GraphVertex *e : vertex->edges) {
      if (!vertex_set.count(e)) {
        vertex_set.emplace(e);
        q.emplace(e);
      }
    }
  }
  for (auto &v : vertex_set) {
    delete v;
  }
}

struct Edge {
  int from;
  int to;
};

template <> struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

void CloneGraphTest(int k, const vector<Edge> &edges) {
  vector<GraphVertex> graph;
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }
  graph.reserve(k);

  for (int i = 0; i < k; i++) {
    graph.push_back(GraphVertex{i});
  }

  for (const Edge &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }
  GraphVertex *result = CloneGraphBFS(&graph[0]);
  CheckGraph(result, graph);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "edges"};
  return GenericTestMain(args, "graph_clone.cc", "graph_clone.tsv",
                         &CloneGraphTest, DefaultComparator{}, param_names);
}
