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

  vstd::vector<int> vec2(20);
  for (int i = 0; i < vec2.size(); ++i) {
    vec2[i] = rand() % 100;
  }
  vec2.push_back(0);
  sort(vec2.begin(), vec2.end());

  // Stack demonstration
  vstd::stack<int> s;
  DEF(s);
  s.push(10);
  s.push(20);
  s.push(30);
  s.push(40);
  s.pop();
  s.pop();
  s.push(50);


  for (int i = 0; i < 10; ++i) {
    if (!vec.empty()) vec[rand() % vec.size()] = rand() % 100;
    if (vec.empty() || rand() % 2) vec.push_back(rand() % 100);
    else vec.pop_back();
  }

  // for (int i = 0; i < 1000; ++i) vec.push_back(i);

  vstd::deque<int> dq;
  DEF(dq);
  dq.push_back(4);
  dq.push_back(20);
  dq.push_front(10);
  dq.pop_back();
  dq.pop_front();
  dq.pop_back();


  MAIN_DONE();
}
