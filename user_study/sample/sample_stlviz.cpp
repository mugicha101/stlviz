#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = {1,4,3};
  DEF(input);

  // ALGORITHM
  vstd::Int output = 0;
  DEF(output);
  for (vstd::Int i = 1; i < input.size(); ++i) {
    DEF(i);
    output += input[i];
    SNAP;
  }

  // OUTPUT - check sum
  MAIN_DONE;
}
