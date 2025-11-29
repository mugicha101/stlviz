#include <vector>
#include <iostream>
#include <algorithm>

int main() {
  // INPUT
  std::vector<int> input = { 14, 6, 19, 1, 3, 9, 13 };
  std::vector<int> queries = { 8, 13, 1, 20 };
  std::vector<int> output;

  // ALGORITHM
  std::sort(input.begin(), input.end());
  for (int q : queries) {
    std::cout << "QUERY: " << q << std::endl;
    int l = 0;
    int r = input.size();
    while (l < r) {
      int m = (l+r) / 2;
      if (input[m] < q) {
        l = m+1;
      } else {
        r = m-1;
      }
    }
    output.push_back(l);
    std::cout << "ANSWER: " << l << std::endl;
  }

  // OUTPUT
  std::cout << "output: [ ";
  for (int x : output) {
    std::cout << x << " ";
  }
  std::cout << "]" << std::endl;
}
