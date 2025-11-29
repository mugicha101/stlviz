#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = { 14, 6, 19, 1, 3, 9, 13 };
  DEF(input);
  vstd::vector<int> queries = { 8, 13, 1, 20 };
  DEF(queries);
  vstd::vector<int> output;
  DEF(output);

  // ALGORITHM
  std::sort(input.begin(), input.end());
  for (vstd::Int q : queries) {
    DEF(q);
    vstd::Int l = 0;
    DEF(l);
    vstd::Int r = input.size();
    DEF(r);
    while (l < r) {
      vstd::Int m = (l+r) / 2;
      DEF(m);
      if (input[m] < q) {
        l = m+1;
      } else {
        r = m-1;
      }
    }
    output.push_back(l);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
