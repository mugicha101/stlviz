#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = { 92, 89, 74, 100, 61, 70, -63, 70, -75, 75 };
  DEF(input);
  vstd::vector<std::pair<int,int>> queries = { {0, 2}, {7, 9}, {4, 4} };
  DEF(queries);

  // ALGORITHM
  vstd::vector<int> psum(1, 0);
  DEF(psum);
  for (vstd::Int i = 0; i < input.size(); ++i) {
    DEF(i);
    psum.push_back(psum.back() + input[i]);
  }
  vstd::vector<int> output;
  DEF(output);
  for (vstd::Int i = 0; i < queries.size(); ++i) {
    DEF(i);
    output.push_back(psum[queries[i].second] - psum[queries[i].first]);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
