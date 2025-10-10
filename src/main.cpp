#include <SFML/Graphics.hpp>
#include <stlviz.hpp>
#include <vobj/primitive.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

// debug program

int main() {
  vstd::vector<int> vec;
  vec.push_back(0);
  vcore::controller.spin();
  std::cout << vobj::Display::displays.size() << std::endl;
}
