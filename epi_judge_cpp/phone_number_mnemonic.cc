#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

vector<string> res;
string builder;

string numToLetter[] = {"0",   "1",   "ABC",  "DEF", "GHI",
                        "JKL", "MNO", "PQRS", "TUV", "WXYZ"};

void dfs(const string &phone_number, int index) {
  if (index >= phone_number.length()) {
    res.push_back(builder);
  } else {
    for (char c : numToLetter[phone_number[index] - '0']) {
      builder.push_back(c);
      dfs(phone_number, index + 1);
      builder.pop_back();
    }
  }
}

vector<string> PhoneMnemonic(const string &phone_number) {
  res.clear();
  builder.clear();
  dfs(phone_number, 0);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"phone_number"};
  return GenericTestMain(args, "phone_number_mnemonic.cc",
                         "phone_number_mnemonic.tsv", &PhoneMnemonic,
                         &UnorderedComparator<std::vector<std::string>>,
                         param_names);
}
