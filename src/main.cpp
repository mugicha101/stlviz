#include <SFML/Graphics.hpp>
#include <stlviz.hpp>
#include <vobj/primitive.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

// debug program

int main() {
  vstd::vector<int> vec(3, 8);
  std::cout << vobj::Display::displays.size() << std::endl;
}
