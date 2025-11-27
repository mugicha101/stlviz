#include <stlviz.hpp>

int main() {
  vstd::vector<int> vec(5, 0);
  DEF(vec);

  vstd::vector<int> vec2(20, 0);
  DEF(vec2);

  MAIN_DONE();
  _Exit(0);
}
