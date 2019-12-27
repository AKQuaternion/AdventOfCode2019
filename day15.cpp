#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <utility>

namespace {
using std::map;
using std::pair;
using std::string;

enum Direction { NORTH = 1, SOUTH = 2, WEST = 3, EAST = 4 };
enum Tile { UNKNOWN, OPEN, WALL, OXYGEN };
map<Direction, Direction> opposite{
    {NORTH, SOUTH}, {SOUTH, NORTH}, {EAST, WEST}, {WEST, EAST}};
map<pair<int, int>, Tile> grid;
int oxyX, oxyY;
void floodFill(Intcode &i, int x, int y);

void explore(Intcode &i, int x, int y, Direction dir) {
  // north (1), south (2), west (3), and east (4).
  // 0: The repair droid hit a wall. Its position has not changed.
  // 1: The repair droid has moved one step in the requested direction.
  // 2: The repair droid has moved one step in the requested direction; its new
  // position is the location of the oxygen system.

  if (grid.count({x, y}) == UNKNOWN) {
    i.run({dir});
    auto out = i.getOutput();
    if (out.back() == 0) // droid hit a wall. Its position has not changed.
      grid[{x, y}] = WALL;
    else if (out.back() == 1) { // droid moved one step
      grid[{x, y}] = OPEN;
      floodFill(i, x, y);
      i.run({opposite[dir]});
    } else { // if out.back() == 2 // droid moved one step and found oxy
      grid[{x, y}] = OXYGEN;
      oxyX = x;
      oxyY = y;
      floodFill(i, x, y);
      i.run({opposite[dir]});
    }
  }
}

void floodFill(Intcode &i, int x, int y) {
  explore(i, x, y + 1, NORTH);
  explore(i, x, y - 1, SOUTH);
  explore(i, x - 1, y, WEST);
  explore(i, x + 1, y, EAST);
}

int bfs(int sx, int sy) {
  int maxd = 0;
  std::set<pair<int, int>> visited;
  std::queue<pair<pair<int, int>, int>> explore;
  explore.push({{sx, sy}, 0});
  while (!explore.empty()) {
    auto [pos, d] = explore.front();
    maxd = d;
    explore.pop();
    visited.insert(pos);
    auto [x, y] = pos;
    if (grid[{x, y}] == OXYGEN) {
      if (d > 0)
        std::cout << "Day 15 star 1 = " << d << "\n";
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
} // namespace

void day15() {
  std::ifstream ifile("../day15.txt");
  Intcode i(ifile);
  grid[{0, 0}] = OPEN;
  floodFill(i, 0, 0);
  bfs(0, 0);

  std::cout << "Day 15 star 2 = " << bfs(oxyX, oxyY) << "\n";
}
