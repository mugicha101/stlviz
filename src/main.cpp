#include <SFML/Graphics.hpp>
#include <stlviz.hpp>
#include <vobj/primitive.hpp>
#include <thread>
#include <iostream>
#include <numeric>

using namespace std::chrono_literals;

// debug program

int main() {
  vstd::vector<int> vec(10, 8);
  vec[1] = 0;
  vec.push_back(10);
  std::iota(vec.begin(), vec.end(), 0);
  vec.pop_back();
}
