#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include <list>
#include <unordered_map>
#include <vector>
using std::list;
using std::make_pair;
using std::prev;
using std::unordered_map;

class LruCache {
public:
  struct Entry {
    list<int>::iterator it;
    int price;
  };

  LruCache(size_t capacity) : capacity(capacity) {}
  int Lookup(int isbn) {
    auto it = Touch(isbn);
    return it != m.end() ? it->second.price : -1;
  }
  void Insert(int isbn, int price) {
    auto it = Touch(isbn);
    // old entry already touched
    if (it == m.end()) {
      // new entry
      // evict if necessary
      if (timeline.size() >= capacity) {
        int oldIsbn = timeline.front();
        timeline.pop_front();
        m.erase(oldIsbn);
      }
      timeline.push_back(isbn);
      auto timeIt = prev(timeline.end());
      Entry ent = {timeIt, price};
      m.insert(make_pair(isbn, ent));
    }
  }
  bool Erase(int isbn) {
    auto it = m.find(isbn);
    if (it != m.end()) {
      Entry ent = it->second;
      timeline.erase(ent.it);
      m.erase(it);
      return true;
    } else {
      return false;
    }
  }

  unordered_map<int, Entry>::iterator Touch(int isbn) {
    auto it = m.find(isbn);
    if (it != m.end()) {
      auto &ent = it->second;
      // remove old it from time
      timeline.erase(ent.it);
      timeline.push_back(isbn);
      ent.it = prev(timeline.end());
    }
    return it;
  }

private:
  unordered_map<int, Entry> m;
  list<int> timeline;
  size_t capacity;
};
struct Op {
  std::string code;
  int arg1;
  int arg2;
};

template <>
struct SerializationTraits<Op> : UserSerTraits<Op, std::string, int, int> {};

void RunTest(const std::vector<Op> &commands) {
  if (commands.empty() || commands[0].code != "LruCache") {
    throw std::runtime_error("Expected LruCache as first command");
  }
  LruCache cache(commands[0].arg1);

  for (int i = 1; i < commands.size(); i++) {
    auto &cmd = commands[i];
    if (cmd.code == "lookup") {
      int result = cache.Lookup(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Lookup: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else if (cmd.code == "insert") {
      cache.Insert(cmd.arg1, cmd.arg2);
    } else if (cmd.code == "erase") {
      bool result = cache.Erase(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Erase: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else {
      throw std::runtime_error("Unexpected command " + cmd.code);
    }
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"commands"};
  return GenericTestMain(args, "lru_cache.cc", "lru_cache.tsv", &RunTest,
                         DefaultComparator{}, param_names);
}
