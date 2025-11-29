#include <vector>
#include <iostream>
#include <set>

int main() {
  // INPUT
  std::vector<int> inputA = { 1, 3, 9, 4 };
  std::vector<int> inputB = { 1, 4, 6, 5, 3 };

  // ALGORITHM
  std::set<int> setA;
  std::set<int> setB;
  for (int i = 0; i < inputA.size(); ++i) {
    setA.insert(inputA[i]);
  }
  for (int i = 0; i < inputA.size(); ++i) {
    setB.insert(inputB[i]);
  }
  std::set<int> setAB;
  for (int x : setA) {
    if (setB.count(x)) {
      setAB.insert(x);
    }
  }
  for (int x : setB) {
    if (setA.count(x)) {
      setAB.insert(x);
    }
  }
  std::vector<int> output;
  for (int x : setAB) {
    output.push_back(x);
  }

  // OUTPUT
  std::cout << "output: [ ";
  for (int i = 0; i < output.size(); ++i) {
    std::cout << output[i] << " ";
  }
  std::cout << "]" << std::endl;
}
