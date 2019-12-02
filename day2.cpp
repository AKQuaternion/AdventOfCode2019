//
// Created by Chris Hartman on 12/1/19.
//
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using std::abs;
using std::ceil;
using std::cout;
using std::endl;
using std::forward_as_tuple;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::map;
using std::max;
using std::max_element;
using std::min;
using std::pair;
using std::queue;
using std::set;
using std::sqrt;
using std::string;
using std::swap;
using std::tie;
using std::tuple;
using std::vector;

void day2() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../input.txt");
  string line;
  vector<int> v;
  while (ifile) {
    string s;
    char c;
    int i;
    ifile >> i >> c;
    v.push_back(i);
  }

  auto v2 = v;
  for (int noun = 0; noun < 100; ++noun)
    for (int verb = 0; verb < 100; ++verb) {
      v = v2;
      v[1] = noun;
      v[2] = verb;
      int pc = 0;
      while (v[pc] != 99) {
        auto op1 = v[pc + 1];
        auto op2 = v[pc + 2];
        auto op3 = v[pc + 3];
        if (v[pc] == 1)
          v[op3] = v[op1] + v[op2];
        else {
          assert(v[pc] == 2);
          v[op3] = v[op1] * v[op2];
        }
        pc += 4;
      }
      if (v[0] == 19690720)
        cout << noun << verb << endl;
    }

  cout << "Day 2 star 1 = " << v[0] << "\n";
//  cout << "Day 2 star 2 = " << noun << verb << "\n";
}
