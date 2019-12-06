//
// Created by Chris Hartman on 12/5/19.
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

auto star1 = 0;

map<string, vector<string>> edges;
map<string, int> ins;

int traverse(string root, int depth) {
  int sum = depth;
  for (auto i : edges[root])
    sum += traverse(i, depth + 1);
  return sum;
}

void day6() {
  auto star2 = 0;
  ifstream ifile("../day6.txt");

  string line;
  while (getline(ifile, line)) {
    string from, to;
    istringstream iline(line);
    getline(iline, from, ')');
    getline(iline, to);
    //    cout << from << " " << to << endl;
    edges[from].push_back(to);
    ins[from];
    ins[to]++;
  }
  for (auto [d, count] : ins)
    if (count == 0)
      cout << d << endl;

  star1 = traverse("COM", 0);
  cout << "Day 6 star 1 = " << star1 << "\n";
  cout << "Day 6 star 2 = " << star2 << "\n";
}

// day 1 calculate fuel required
// day 2 run IntCode program
// day 3 wires on a grid U D L R
// COM)B
// B)C
// C)D
// D)E
// E)F
// B)G
// G)H
// D)I
// E)J
// J)K
// K)L