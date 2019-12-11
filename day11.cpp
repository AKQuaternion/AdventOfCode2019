//
// Created by Chris Hartman on 12/10/19.
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
using Position = std::pair<int, int>;
void day11() {
  static vector<Position> dirs = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

  auto star1 = 0;
  auto star2 = 0;
  auto dir = 0;
  Position pos{0, 0};
  map<Position, int> grid;
  //  grid[{0,0}] = 1;
  ifstream ifile("../day11.txt");
  Intcode i(ifile);
  pair<Intcode::State, long long> res{Intcode::INPUT, 0};
  while (res.first != Intcode::HALT) {
    //    cout << "(" << pos.first << "," << pos.second << ") " << grid[pos] <<
    //    endl;
    if (grid.count(pos) == 1)
      res = i.run({grid[pos]});
    else
      res = i.run({0});
    if (res.first == Intcode::HALT) {
      cout << "xxxxx";
      break;
    }
    cout << "paint " << res.second << endl;
    grid[pos] = res.second;
    res = i.run();
    if (res.first == Intcode::HALT)
      break;
    assert(res.second == 1 || res.second == 0);
    cout << "turn " << res.second << endl;
    dir = (dir + (res.second == 0 ? 1 : 3)) % 4;
    pos.first += dirs[dir].first;
    pos.second += dirs[dir].second;
  }

  auto count = 0;
  for (auto [pos, v] : grid) {
    cout << ++count << " (" << pos.first << "," << pos.second << ") " << v
         << endl;
  }

  cout << "Day 11 star 1 = " << grid.size() << "\n";

  for (int y = 50; y > -50; --y) {
    for (int x = -50; x < 50; ++x)
      cout << (grid[{x, y}] == 0 ? ' ' : '*');
    cout << endl;
  }

  cout << "Day 11 star 2 = " << star2 << "\n";
} // not 3089
