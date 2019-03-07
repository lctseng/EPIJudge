#include "test_framework/generic_test.h"
#include <climits>
#include <map>
#include <vector>
using std::make_pair;
using std::multimap;
using std::pair;
using std::size;
using std::vector;

int FindClosestElementsInSortedArrays(
    const vector<vector<int>> &sorted_arrays) {
  multimap<int, pair<int, int>> countMap; // value to its array id and index
  for (int i = 0; i < size(sorted_arrays); i++) {
    countMap.insert(make_pair(sorted_arrays[i][0], make_pair(i, 0)));
  }
  // until one list is empty
  int minDistance = INT_MAX;
  while (true) {
    int currentDistance = prev(countMap.end())->first - countMap.begin()->first;
    if (currentDistance < minDistance)
      minDistance = currentDistance;
    // extract smallest and advance
    auto minIt = countMap.begin();
    auto minData = minIt->second;
    countMap.erase(minIt);
    // check termination
    if (++minData.second == sorted_arrays[minData.first].size()) {
      break;
    }
    countMap.insert(
        make_pair(sorted_arrays[minData.first][minData.second], minData));
  }

  return minDistance;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "minimum_distance_3_sorted_arrays.cc",
                         "minimum_distance_3_sorted_arrays.tsv",
                         &FindClosestElementsInSortedArrays,
                         DefaultComparator{}, param_names);
}
