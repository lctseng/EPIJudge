#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <iterator>
#include <vector>
using std::sort;
using std::vector;

class Team {
public:
  explicit Team(const vector<int> &height) {
    transform(begin(height), end(height), back_inserter(players_),
              [](int h) { return Player{h}; });
  }
  // Checks if team0 can be placed in front of team1.
  static bool ValidPlacementExists(const Team &team0, const Team &team1) {
    vector<Player> team0Players = team0.players_;
    vector<Player> team1Players = team1.players_;
    sort(team0Players.begin(), team0Players.end());
    sort(team1Players.begin(), team1Players.end());
    for (int i = 0; i < team0Players.size(); i++) {
      if (team1Players[i].height <= team0Players[i].height)
        return false;
    }
    return true;
  }

private:
  struct Player {
    bool operator<(const Player &that) const { return height < that.height; }

    int height;
  };
  vector<Player> players_;
};
void ValidPlacementExistsWrapper(TimedExecutor &executor,
                                 const vector<int> &team0,
                                 const vector<int> &team1, bool expected_01,
                                 bool expected_10) {
  Team t0(team0), t1(team1);

  bool result_01 =
      executor.Run([&] { return Team::ValidPlacementExists(t0, t1); });
  bool result_10 =
      executor.Run([&] { return Team::ValidPlacementExists(t1, t0); });
  if (result_01 != expected_01 || result_10 != expected_10) {
    throw TestFailure("");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "team0", "team1",
                                       "expected_01", "expected_10"};
  return GenericTestMain(args, "is_array_dominated.cc",
                         "is_array_dominated.tsv", &ValidPlacementExistsWrapper,
                         DefaultComparator{}, param_names);
}
