#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <deque>
#include <vector>
using std::deque;
using std::make_pair;
using std::pair;
using std::vector;

void FlipColor(int x, int y, vector<deque<bool>> *image_ptr) {
  // record the color we want
  bool toBeFlip = (*image_ptr)[x][y];
  bool targetColor = !toBeFlip;
  int m = image_ptr->size();
  int n = image_ptr->at(0).size();
  deque<pair<int, int>> q;
  (*image_ptr)[x][y] = targetColor;
  const int SHIFT[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
  q.emplace_back(x, y);
  while (q.size()) {
    auto current = q.front();
    q.pop_front();
    // find next
    for (int i = 0; i < 4; i++) {
      int nextX = current.first + SHIFT[i][0];
      int nextY = current.second + SHIFT[i][1];
      if (nextX >= 0 && nextX < m && nextY >= 0 && nextY < n &&
          image_ptr->at(nextX)[nextY] == toBeFlip) {
        q.emplace_back(nextX, nextY);
        image_ptr->at(nextX)[nextY] = targetColor;
      }
    }
  }
}
vector<vector<int>> FlipColorWrapper(TimedExecutor &executor, int x, int y,
                                     vector<vector<int>> image) {
  vector<deque<bool>> b;
  b.reserve(image.size());
  for (const vector<int> &row : image) {
    deque<bool> tmp;
    tmp.resize(row.size());
    for (int i = 0; i < row.size(); ++i) {
      tmp[i] = static_cast<bool>(row[i]);
    }
    b.push_back(tmp);
  }

  executor.Run([&] { FlipColor(x, y, &b); });

  image.resize(b.size());

  for (int i = 0; i < image.size(); ++i) {
    image[i].resize(b.size());
    for (int j = 0; j < image[i].size(); ++j) {
      image[i][j] = b[i][j];
    }
  }
  return image;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "x", "y", "image"};
  return GenericTestMain(args, "matrix_connected_regions.cc", "painting.tsv",
                         &FlipColorWrapper, DefaultComparator{}, param_names);
}
