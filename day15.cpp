//
// Created by Chris Hartman on 12/14/19.
//
#include "Intcode.hpp"

#include <fstream>
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
using std::string;
using std::swap;
using std::tie;
using std::tuple;
using std::vector;

// north (1), south (2), west (3), and east (4).
// 0: The repair droid hit a wall. Its position has not changed.
// 1: The repair droid has moved one step in the requested direction.
// 2: The repair droid has moved one step in the requested direction; its new
// position is the location of the oxygen system.
// My codes:
// 0 unknown
// 1 known open
// 2 known wall
// 3 Oxygen
map<pair<int, int>, int> grid;
int oxyX, oxyY;
void printit() {
  grid[{0, 0}] = 'o';
  const int sz = 80;
  for (int row = -sz; row < sz; ++row) {
    for (int col = -sz; col < sz; ++col)
      switch (grid[{col, row}]) {
      case 0:
        cout << ' ';
        break;
      case 1:
        cout << '.';
        break;
      case 2:
        cout << '#';
        break;
      case 3:
        cout << 'x';
        break;
      case 'o':
        cout << 'o';
        break;
      default:
        throw 1;
      }
    cout << "\n";
  }
  grid[{0, 0}] = '.';
}
void fill(Intcode &i, int x, int y);

void explore(Intcode &i, int x, int y, int dir) {
  int opposite[] = {0, 2, 1, 4, 3};
  if (grid.count({x, y}) == 0) {
    i.run({dir});
    auto out = i.getOutput();
    if (out.back() == 0)
      grid[{x, y}] = 2;
    else if (out.back() == 1) {
      grid[{x, y}] = 1;
      fill(i, x, y);
      i.run({opposite[dir]});
    } else { // if out.back() == 2
      grid[{x, y}] = 3;
      //      cout << "Oxy at " << x << ", " << y << endl;
      oxyX = x;
      oxyY = y;
      fill(i, x, y);
      i.run({opposite[dir]});
    }
  }
}

void fill(Intcode &i, int x, int y) {
  explore(i, x, y + 1, 1);
  explore(i, x, y - 1, 2);
  explore(i, x - 1, y, 3);
  explore(i, x + 1, y, 4);
}

int bfs(int sx, int sy) {
  int maxd = 0;
  set<pair<int, int>> visited;
  queue<pair<pair<int, int>, int>> explore;
  explore.push({{sx, sy}, 0});
  while (!explore.empty()) {
    auto [pos, d] = explore.front();
    maxd = d;
    explore.pop();
    visited.insert(pos);
    auto [x, y] = pos;
    //    cout << "exploring " << x << " " << y << " d=" << d << endl;
    if (grid[{x, y}] == 3) {
      if (d > 0)
        cout << "Day 15 star 1 = " << d << "\n";
    }
    if (grid[{x, y + 1}] % 2 == 1 && visited.count({x, y + 1}) == 0)
      explore.push({{x, y + 1}, d + 1});
    if (grid[{x, y - 1}] % 2 == 1 && visited.count({x, y - 1}) == 0)
      explore.push({{x, y - 1}, d + 1});
    if (grid[{x + 1, y}] % 2 == 1 && visited.count({x + 1, y}) == 0)
      explore.push({{x + 1, y}, d + 1});
    if (grid[{x - 1, y}] % 2 == 1 && visited.count({x - 1, y}) == 0)
      explore.push({{x - 1, y}, d + 1});
  }
  return maxd;
}

void day15() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day15.txt");
  Intcode i(ifile);
  grid[{0, 0}] = 1;
  fill(i, 0, 0);
  //    printit();
  bfs(0, 0);
  star2 = bfs(oxyX, oxyY);

  cout << "Day 15 star 2 = " << star2 << "\n";
}
