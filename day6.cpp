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

map<string, vector<string>> edges;
map<string, int> visited;

int traverse(string root, int depth) {
  visited[root] = depth;
  int sum = depth;
  for (auto i : edges[root])
    if (visited.count(i) == 0)
      sum += traverse(i, depth + 1);
  return sum;
}

void day6() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day6.txt");

  string line;
  while (getline(ifile, line)) {
    string from, to;
    istringstream iline(line);
    getline(iline, from, ')');
    getline(iline, to);
    edges[from].push_back(to);
    edges[to].push_back(from);
  }

  star1 = traverse("COM", 0);
  visited.clear();

  traverse("YOU", 0);
  star2 = visited["SAN"] - 2;
  cout << "Day 6 star 1 = " << star1 << "\n";
  cout << "Day 6 star 2 = " << star2 << "\n";
}