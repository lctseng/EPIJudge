#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include <map>
#include <string>
#include <unordered_map>
using std::make_pair;
using std::multimap;
using std::string;
using std::unordered_map;

class ClientsCreditsInfo {
public:
  void Insert(const string &client_id, int c) {
    auto oldIt = clientTreeMap.find(client_id);
    if (oldIt != clientTreeMap.end()) {
      // remove prev entry
      creditMap.erase(oldIt->second);
    }
    // add new
    auto it = creditMap.insert(make_pair(c, client_id));
    if (oldIt != clientTreeMap.end()) {
      oldIt->second = it;
    } else {
      clientTreeMap[client_id] = it;
    }
  }
  bool Remove(const string &client_id) {
    auto oldIt = clientTreeMap.find(client_id);
    if (oldIt != clientTreeMap.end()) {
      creditMap.erase(oldIt->second);
      clientTreeMap.erase(oldIt);
      return true;
    } else {
      return false;
    }
  }
  int Lookup(const string &client_id) const {
    auto hashIt = clientTreeMap.find(client_id);
    if (hashIt != clientTreeMap.end()) {
      return hashIt->second->first;
    } else {
      return -1;
    }
  }
  void AddAll(int C) {
    // iterate current clients
    for (auto &data : clientTreeMap) {
      // data: string, iterator
      int newCredit = data.second->first + C;
      // update tree it
      creditMap.erase(data.second);
      data.second = creditMap.insert(make_pair(newCredit, data.first));
    }
    return;
  }
  string Max() const {
    if (creditMap.empty())
      return "";
    return prev(creditMap.end())->second;
  }

private:
  multimap<int, string> creditMap;
  unordered_map<string, multimap<int, string>::iterator> clientTreeMap;
};
struct Operation {
  std::string op;
  std::string s_arg;
  int i_arg;
};

std::ostream &operator<<(std::ostream &out, const Operation &op) {
  return out << FmtStr("{}({}, {})", op.op, op.s_arg, op.i_arg);
}

template <>
struct SerializationTraits<Operation>
    : UserSerTraits<Operation, std::string, std::string, int> {};

void ClientsCreditsInfoTester(const std::vector<Operation> &ops) {
  ClientsCreditsInfo cr;
  int op_idx = 0;
  for (auto &op : ops) {
    if (op.op == "ClientsCreditsInfo") {
      continue;
    } else if (op.op == "remove") {
      bool result = cr.Remove(op.s_arg);
      if (result != op.i_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);
      }
    } else if (op.op == "max") {
      auto result = cr.Max();
      if (result != op.s_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);
      }
    } else if (op.op == "insert") {
      cr.Insert(op.s_arg, op.i_arg);
    } else if (op.op == "add_all") {
      cr.AddAll(op.i_arg);
    } else if (op.op == "lookup") {
      auto result = cr.Lookup(op.s_arg);
      if (result != op.i_arg) {
        throw TestFailure()
            .WithProperty(PropertyName::STATE, cr)
            .WithProperty(PropertyName::COMMAND, op)
            .WithMismatchInfo(op_idx, op.i_arg, result);
        ;
      }
    }
    op_idx++;
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "adding_credits.cc", "adding_credits.tsv",
                         &ClientsCreditsInfoTester, DefaultComparator{},
                         param_names);
}
