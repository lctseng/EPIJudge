#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using std::map;
using std::string;
using std::swap;
using std::unordered_map;
using std::vector;

struct Person {
  int age;
  string name;
};

// sorted version: O(n + mlogm)
void GroupByAge(vector<Person> *peoplePtr) {
  auto &people = *peoplePtr;
  // count the number of element, accumulate the offset
  map<int, int> elementCount;
  for (auto &person : people) {
    elementCount[person.age]++;
  }
  int offset = 0;
  unordered_map<int, int> elementOffset;
  for (auto &dataPair : elementCount) {
    elementOffset[dataPair.first] = offset;
    offset += dataPair.second;
  }
  // in-place counting sort
  // while is not empty
  while (elementOffset.size()) {
    // extract a number and its target position
    int from = elementOffset.begin()->second;
    int ageToMove = people[from].age;
    int to = elementOffset[ageToMove];
    // move the element in my target to the right place
    swap(people[from], people[to]);
    // increment the offset, decrease the counter, remove if zero
    if (--elementCount[ageToMove] == 0) {
      elementOffset.erase(ageToMove);
    } else {
      elementOffset[ageToMove]++;
    }
  }
}

// void GroupByAge(vector<Person> *people) {
//   unordered_map<int, int> ageCount;
//   for (auto &person : *people) {
//     ageCount[person.age]++;
//   }
//   // compute end-offset by accumulating
//   unordered_map<int, int> ageOffset;
//   int currentOffset = 0;
//   for (auto it = ageCount.begin(); it != ageCount.end(); ++it) {
//     ageOffset[it->first] = currentOffset;
//     currentOffset += it->second;
//   }
//   // update in-place
//   while (ageOffset.size()) {
//     // move the person occupied at from to its correct position
//     auto from = ageOffset.begin();
//     auto personAgeToMove = (*people)[from->second].age;
//     auto &toIndex = ageOffset[personAgeToMove];
//     swap(people->at(from->second), people->at(toIndex));
//     if (--ageCount[personAgeToMove] > 0) {
//       ++toIndex;
//     } else {
//       ageOffset.erase(personAgeToMove);
//     }
//   }
// }

template <>
struct SerializationTraits<Person> : UserSerTraits<Person, int, string> {};

void GroupByAgeWrapper(TimedExecutor &executor, vector<Person> &people) {
  if (people.empty()) {
    return;
  }
  std::multiset<Person, std::function<bool(Person, Person)>> values(
      begin(people), end(people), [](const Person &a, const Person &b) {
        return a.age == b.age ? a.name < b.name : a.age < b.age;
      });

  executor.Run([&] { GroupByAge(&people); });

  if (people.empty()) {
    throw TestFailure("Empty result");
  }
  std::set<int> ages;
  int last_age = people[0].age;
  for (auto &x : people) {
    if (ages.count(x.age) != 0) {
      throw TestFailure("Entries are not grouped by age");
    }
    if (x.age != last_age) {
      ages.insert(last_age);
      last_age = x.age;
    }
    auto it = values.find(x);
    if (it == end(values)) {
      throw TestFailure("Entry set changed");
    }
    values.erase(it);
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "people"};
  return GenericTestMain(args, "group_equal_entries.cc",
                         "group_equal_entries.tsv", &GroupByAgeWrapper,
                         DefaultComparator{}, param_names);
}
