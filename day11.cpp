//
// Created by Chris Hartman on 12/10/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <utility>
#include <vector>
using std::cout;
using std::ifstream;
using std::map;
using std::pair;
using std::vector;
using Position = std::pair<int, int>;

map<Position, int> paint(int startColor, Intcode &i) {
  static const vector<Position> dirs = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  Position pos{0, 0};
  auto dir = 0;
  map<Position, int> grid;
  grid[pos] = startColor;
  while (true) {
    auto input = (grid.count(pos) == 0) ? 0 : grid[pos];
    auto state = i.run({input});
    if (state == Intcode::HALT)
      return grid;
    auto out = i.getOutput();
    for (int i = 0; i < out.size(); i += 2) {
      grid[pos] = out[i];
      dir = (dir + (out[i + 1] == 0 ? 1 : 3)) % 4;
      pos.first += dirs[dir].first;
      pos.second += dirs[dir].second;
    }
  }
}

void day11() {
  ifstream ifile("../day11.txt");
  Intcode i(ifile);

  auto grid = paint(0, i);
  cout << "Day 11 star 1 = " << grid.size() << "\n";

  i.reset();
  grid = paint(1, i);
  auto whitePos = *std::find_if(grid.begin(), grid.end(),
                                [](auto p) { return p.second == 1; });
  int minx = whitePos.first.first, maxx = whitePos.first.first;
  int miny = whitePos.first.second, maxy = whitePos.first.second;
  for (auto [pos, c] : grid) {
    if (c != 1)
      continue;
    auto [px, py] = pos;
    minx = std::min(minx, px);
    miny = std::min(miny, py);
    maxx = std::max(maxx, px);
    maxy = std::max(maxy, py);
  }
  for (int y = maxy; y >= miny; --y) {
    for (int x = minx; x <= maxx; ++x)
      cout << (grid[{x, y}] == 0 ? "  " : "* ");
    cout << "\n";
  }
}
