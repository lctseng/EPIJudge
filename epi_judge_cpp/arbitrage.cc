#include "test_framework/generic_test.h"
#include <limits>
#include <vector>
using std::vector;

bool BFNegativeCycleDetection(vector<vector<double>> &graph, int src) {
  int nodeNum = graph.size();
  vector<double> distanceToSource(nodeNum, std::numeric_limits<double>::max());
  distanceToSource[src] = 0;
  // perorm nodeNum - 1 time relaxation
  for (int i = 1; i < nodeNum; i++) {
    // try each edge
    bool haveUpdated = false;
    for (int a = 0; a < nodeNum; a++) {
      for (int b = 0; b < nodeNum; b++) {
        // d[a] + w[a][b] <=> d[b]
        if (distanceToSource[a] + graph[a][b] < distanceToSource[b]) {
          distanceToSource[b] = distanceToSource[a] + graph[a][b];
          haveUpdated = true;
        }
      }
    }
    if (!haveUpdated) {
      // does not update since this relaxation: stable
      // impossible to have negative cycle
      return false;
    }
  }
  // try again relaxation, if can, means negative cycle exists
  for (int a = 0; a < nodeNum; a++) {
    for (int b = 0; b < nodeNum; b++) {
      // d[a] + w[a][b] <=> d[b]
      if (distanceToSource[a] + graph[a][b] < distanceToSource[b]) {
        return true;
      }
    }
  }
  return false;
}

bool IsArbitrageExist(vector<vector<double>> graph) {
  // convert weight to new graph to -lg(e)
  for (auto &arr : graph) {
    for (auto &w : arr) {
      w = -log(w);
    }
  }
  // perform BF negative cycle detection
  // sincle graph is complete, choose any node will work
  return BFNegativeCycleDetection(graph, 0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"graph"};
  return GenericTestMain(args, "arbitrage.cc", "arbitrage.tsv",
                         &IsArbitrageExist, DefaultComparator{}, param_names);
}
