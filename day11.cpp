#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <utility>
#include <vector>

using Position = std::pair<int, int>;

std::map<Position, int> paint(int startColor, Intcode &i) {
  static const std::vector<Position> dirs = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  Position pos{0, 0};
  auto dir = 0;
  std::map<Position, int> grid;
  grid[pos] = startColor;
  while (true) {
    auto input = (grid.count(pos) == 0) ? 0 : grid[pos];
    auto state = i.run({input});
    if (state == Intcode::HALT)
      return grid;
    auto out = i.getOutput();
    for (int o = 0; o < out.size();) {
      grid[pos] = int(out[o++]);
      dir = (dir + (out[o++] == 0 ? 1 : 3)) % 4;
      pos.first += dirs[dir].first;
      pos.second += dirs[dir].second;
    }
  }
}

void day11() {
  std::ifstream ifile("../day11.txt");
  Intcode i(ifile);

  auto grid = paint(0, i);
  std::cout << "Day 11 star 1 = " << grid.size() << "\n";

  i.reset();
  grid = paint(1, i);
  auto whitePos = *std::find_if(grid.begin(), grid.end(),
                                [](auto p) { return p.second == 1; });
  int minx = whitePos.first.first, maxx = whitePos.first.first;
  int miny = whitePos.first.second, maxy = whitePos.first.second;
  for (auto [pos, c] : grid)
    if (c == 1) {
      auto [px, py] = pos;
      minx = std::min(minx, px);
      miny = std::min(miny, py);
      maxx = std::max(maxx, px);
      maxy = std::max(maxy, py);
    }
  std::cout << "Day 11 star 2 =\n";
  for (int y = maxy; y >= miny; --y) {
    for (int x = minx; x <= maxx; ++x)
      std::cout << (grid[{x, y}] == 0 ? "  " : "* ");
    std::cout << "\n";
  }
}
