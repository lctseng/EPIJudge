#include "test_framework/generic_test.h"
#include <deque>
#include <string>
using std::deque;
using std::string;

const string kCurrent = ".";
const string kPrev = "..";

void addSubPath(deque<string> &pathStack, string &current) {
  if (current == kCurrent) {
    // nothing
  } else if (current == kPrev && pathStack.size() &&
             pathStack.back() != kPrev) {
    pathStack.pop_back();
  } else {
    pathStack.push_back(move(current));
  }
}

string ShortestEquivalentPath(const string &path) {
  if (path.empty())
    return path;
  deque<string> pathStack;
  size_t pos = 0;
  bool absolute = false;
  if (path[0] == '/') {
    absolute = true;
    ++pos;
  }
  // traverse
  size_t start = pos;
  while ((pos = path.find_first_of('/', start)) != string::npos) {
    if (pos - start > 0) {
      // have a path here
      string current = path.substr(start, pos - start);
      addSubPath(pathStack, current);
    }
    start = pos + 1;
  }
  // push last entry
  if (start < path.length()) {
    string current = path.substr(start);
    addSubPath(pathStack, current);
  }
  // convert path to stack
  string res;
  if (absolute)
    res.append(1, '/');
  for (auto &s : pathStack) {
    res.append(s);
    res.append(1, '/');
  }
  if (res.size() > 1)
    res.pop_back(); // remove final / if size > 1
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"path"};
  return GenericTestMain(args, "directory_path_normalization.cc",
                         "directory_path_normalization.tsv",
                         &ShortestEquivalentPath, DefaultComparator{},
                         param_names);
}
