#include <stlviz.hpp>

int main() {
  // INPUT
  std::vector<int> inputA = { 1, 3, 9, 4 };
  std::vector<int> inputB = { 1, 4, 6, 5, 3 };

  // ALGORITHM
  vstd::set<int> setA;
  DEF(setA);
  vstd::set<int> setB;
  DEF(setB);
  for (vstd::Int i = 0; i < inputA.size(); ++i) {
    DEF(i);
    setA.insert(inputA[i]);
  }
  for (vstd::Int i = 0; i < inputA.size(); ++i) {
    DEF(i);
    setB.insert(inputB[i]);
  }
  vstd::set<int> setAB;
  DEF(setAB);
  for (vstd::Int x : setA) {
    DEF(x);
    if (setB.count(x)) {
      setAB.insert(x);
    }
  }
  for (vstd::Int x : setB) {
    DEF(x);
    if (setA.count(x)) {
      setAB.insert(x);
    }
  }
  vstd::vector<int> output;
  DEF(output);
  for (vstd::Int x : setAB) {
    DEF(x);
    output.push_back(x);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
