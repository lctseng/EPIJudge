#include "test_framework/generic_test.h"
#include <limits>
#include <vector>
using std::vector;
bool BFNegativeCycleDetection(vector<vector<double>> &graph, int src) {
  int V = graph.size();
  vector<double> distanceToSrc(V, std::numeric_limits<double>::max());
  distanceToSrc[src] = 0;
  // perform V-1 relaxation
  // to find single-source shortest path
  for (int v = 1; v < V; v++) {
    bool hasUpdate = false;
    for (int i = 0; i < V; i++) {
      for (int j = 0; j < V; j++) {
        // if src can reach i
        // try go this to update distance to j
        if (distanceToSrc[i] + graph[i][j] < distanceToSrc[j]) {
          distanceToSrc[j] = distanceToSrc[i] + graph[i][j];
          hasUpdate = true;
        }
      }
    }
    // if no update in one relaxation, no negative cycle
    if (!hasUpdate)
      return false;
  }
  // after V-1 times, perform one more time
  for (int i = 0; i < V; i++) {
    for (int j = 0; j < V; j++) {
      if (distanceToSrc[i] + graph[i][j] < distanceToSrc[j]) {
        return true;
      }
    }
  }
  // if any, we have negative cycle
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
