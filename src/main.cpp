#include <stlviz.hpp>
#include <numeric>

// debug program

int main() {
  vstd::vector<int> vec(10, 8);
  DEF(vec);
  vec[1] = 0;
  vec.push_back(10);
  std::iota(vec.begin(), vec.end(), 0);
  vec.pop_back();

  vstd::vector<int> vec2;
  DEF(vec2);
  vec2.push_back(1);
}
