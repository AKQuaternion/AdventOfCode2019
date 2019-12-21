//
// Created by Chris Hartman on 12/20/19.
//
//
// Created by Chris Hartman on 12/16/19.
//
#include "Intcode.hpp"

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

namespace {
ifstream ifile("../day21.txt");
Intcode i(ifile);
} // namespace

void day21() {
  auto star1 = 0;
  auto star2 = 0;
  // T and J start false
  // ABCD false if hole at 1234, true if ground
  // AND X Y is Y &= X
  // OR X Y is  Y |= X
  // NOT X Y is Y = ~X
  string program{""
                 "NOT C T\n"
                 "AND D T\n"
                 "OR T J\n"
                 "NOT A T\n" // if A is bad, jump
                 "OR T J\n"
                 "WALK\n"
                 ""};
  vector<long long> p;
  copy(program.begin(), program.end(), std::back_inserter(p));
  i.run(p);
  auto out = i.getOutput();
  for (auto n : out)
    cout << n << endl;
  string asciiOutput;
  std::copy(out.begin(), out.end(), std::back_inserter(asciiOutput));
  cout << asciiOutput;
}
