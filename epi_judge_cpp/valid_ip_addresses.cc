#include "test_framework/generic_test.h"
#include <algorithm>
#include <string>
#include <vector>
using std::min;
using std::move;
using std::pow;
using std::stoi;
using std::string;
using std::vector;

bool IsValidPart(const string &s, int begin, int end) {
  int sz = end - begin;
  if (sz <= 0 || sz > 3)
    return false;
  // leading zero check
  if (sz > 1 && s[begin] == '0')
    return false;
  // num check
  // BE CAREFUL the order
  int num = 0;
  for (int i = begin; i < end; i++) {
    num = num * 10 + s[i] - '0';
  }
  return num <= 255;
}

// Easy
vector<string> GetValidIpAddress(const string &s) {
  int sz = s.length();
  vector<string> res;
  // find 3 partition point
  for (int p1 = 0; p1 <= 2; p1++) {
    if (IsValidPart(s, 0, p1 + 1))
      for (int p2 = p1 + 1; p2 <= p1 + 3 && p2 < sz; p2++) {
        if (IsValidPart(s, p1 + 1, p2 + 1))
          for (int p3 = p2 + 1; p3 <= p2 + 3 && p3 < sz; p3++) {
            if (IsValidPart(s, p2 + 1, p3 + 1) && IsValidPart(s, p3 + 1, sz)) {
              // add answer
              string current;
              int i = 0;
              for (; i <= p1; i++) {
                current.push_back(s[i]);
              }
              current.append(1, '.');
              for (; i <= p2; i++) {
                current.push_back(s[i]);
              }
              current.append(1, '.');
              for (; i <= p3; i++) {
                current.push_back(s[i]);
              }
              current.append(1, '.');
              for (; i < sz; i++) {
                current.push_back(s[i]);
              }
              res.emplace_back(move(current));
            }
          }
      }
  }
  return res;
}

vector<string> GetValidIpAddressFast(const string &s) {
  // find 3 cutting period
  int sz = s.length();
  vector<string> res;
  int num1 = 0;
  for (int p1 = 0; p1 <= 2; p1++) {
    // check leading zero for num1
    // this MUST go before number accumulation
    if (p1 > 0 && num1 == 0)
      break;
    // Note that this number computing MUST go before any other pruning
    num1 = num1 * 10 + s[p1] - '0';
    // early terminate: when remaining string is too long
    if (sz - p1 - 1 > 9)
      continue;
    // early terminate when num1 is too large
    if (num1 > 255)
      break;
    // setup upper bound for p2
    int maxP2 = min(p1 + 3, sz - 3);
    int num2 = 0;
    for (int p2 = p1 + 1; p2 <= maxP2; p2++) {
      // check leading zero for num2
      if (p2 > p1 + 1 && num2 == 0)
        break;
      num2 = num2 * 10 + s[p2] - '0';
      if (num2 > 255 || (num2 == 0 && p2 > p1 + 1))
        break;
      if (sz - p2 - 1 > 6)
        continue;
      int maxP3 = min(p2 + 3, sz - 2);
      int num3 = 0, num4 = stoi(s.substr(p2 + 1));
      // this EXP does not affected when num4 has leading zeros
      // computed directly from number of digits of num4
      int num4Exp = pow(10, sz - p2 - 2);
      for (int p3 = p2 + 1; p3 <= maxP3; p3++) {
        // check leading zero for num3
        if (p3 > p2 + 1 && num3 == 0)
          break;
        num3 = num3 * 10 + s[p3] - '0';
        num4 %= num4Exp;
        num4Exp /= 10;
        // check leading zero for num4
        if (num4Exp >= 10 && num4 < num4Exp)
          continue;
        if (sz - p3 - 1 > 3)
          continue;
        if (num3 > 255)
          break;
        if (num4 > 255)
          continue;
        // add answer
        string current;
        int i = 0;
        for (; i <= p1; i++) {
          current.push_back(s[i]);
        }
        current.append(1, '.');
        for (; i <= p2; i++) {
          current.push_back(s[i]);
        }
        current.append(1, '.');
        for (; i <= p3; i++) {
          current.push_back(s[i]);
        }
        current.append(1, '.');
        for (; i < sz; i++) {
          current.push_back(s[i]);
        }
        res.emplace_back(move(current));
      }
    }
  }
  return res;
}

bool comp(vector<string> expected, vector<string> result) {
  std::sort(begin(expected), end(expected));
  std::sort(begin(result), end(result));
  return expected == result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "valid_ip_addresses.cc",
                         "valid_ip_addresses.tsv", &GetValidIpAddress, &comp,
                         param_names);
}
