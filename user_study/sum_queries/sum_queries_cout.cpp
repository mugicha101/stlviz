#include <vector>
#include <iostream>
#include <utility>

int main() {
  // INPUT
  std::vector<int> input = { 92, 89, 74, 100, 61, 70, -63, 70, -75, 75 };
  std::vector<std::pair<int,int>> queries = { {0, 2}, {7, 9}, {4, 4} };

  // ALGORITHM
  std::vector<int> psum(1, 0);
  for (int i = 0; i < input.size(); ++i) {
    psum.push_back(psum.back() + input[i]);
  }
  std::vector<int> output;
  for (int i = 0; i < queries.size(); ++i) {
    output.push_back(psum[queries[i].second] - psum[queries[i].first]);
  }

  // OUTPUT
  std::cout << "output: [ ";
  for (int x : output) {
    std::cout << x << " ";
  }
  std::cout << "]" << std::endl;
}
