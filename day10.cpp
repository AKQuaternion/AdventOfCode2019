#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using std::string;
using std::vector;
using std::pair;

bool onGrid(const vector<string> &g, int y, int x) {
  return (y >= 0 && y < g.size() && x >= 0 && x < g.size());
}

// fire returns the position of asteroid hit when firing a laser from bc, br
// in the direction dx, dy, or returns {-1,-1} if no asteroid is hit
pair<int, int> fire(const vector<string> &g, int bc, int br, int dx, int dy) {
  for (int t = 1; true; ++t)
    if (onGrid(g, br + t * dy, bc + t * dx)) {
      if (g[br + t * dy][bc + t * dx] == '#')
        return {bc + t * dx, br + t * dy};
    } else
      return {-1, -1};
}

void day10() {
  auto star1 = 0, star2 = 0;
  std::ifstream ifile("../day10.txt");
  string line;
  vector<string> grid;
  while (getline(ifile, line))
    grid.push_back(line);

  vector<pair<int, int>> slopes;
  for (int dx = -grid.size(); dx <= int(grid.size()); ++dx)
    for (int dy = -grid.size(); dy <= int(grid.size()); ++dy)
      if (std::gcd(dx, dy) == 1)
        slopes.emplace_back(dx, dy);

  sort(slopes.begin(), slopes.end(), [](auto s1, auto s2) {
    return atan2(s1.first, s1.second) > atan2(s2.first, s2.second);
  });

  int bestRow = -1, bestCol = -1;

  for (auto y = 0ul; y < grid.size(); ++y)
    for (auto x = 0ul; x < grid.size(); ++x)
      if (grid[y][x] == '#') {
        auto detected = count_if(slopes.begin(), slopes.end(), [&](auto s) {
          return fire(grid, x, y, s.first, s.second) != pair<int, int>{-1, -1};
        });
        if (detected > star1) {
          star1 = detected;
          bestCol = x;
          bestRow = y;
        }
      }

  int count = 0;
  while (star2 == 0)
    for (auto [dx, dy] : slopes) {
      auto hitPos = fire(grid, bestCol, bestRow, dx, dy);
      if (hitPos != pair<int, int>{0, 0}) {
        grid[hitPos.second][hitPos.first] = ' ';
        if (++count == 200)
          star2 = hitPos.first * 100 + hitPos.second;
      }
    }

  std::cout << "Day 10 star 1 = " << star1 << "\n";
  std::cout << "Day 10 star 2 = " << star2 << "\n";
}