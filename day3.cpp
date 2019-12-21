#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <utility>

using Position = std::pair<int, int>;

std::map<Position, int> pathToGrid(const std::string &path) {
  static std::map<char, Position> dirs = {
      {'U', {0, 1}}, {'D', {0, -1}}, {'L', {-1, 0}}, {'R', {1, 0}}};

  std::map<Position, int> out;

  std::istringstream in(path);
  char dir, comma;
  int steps;
  int posX = 0, posY = 0;
  int step = 0;
  while (in >> dir >> steps >> comma)
    for (int s = 0; s < steps; ++s) {
      posX += dirs[dir].first;
      posY += dirs[dir].second;
      out[{posX, posY}] = ++step;
    }
  return out;
}

void day3() {
  std::ifstream ifile("../day3.txt");
  std::string line1, line2;
  ifile >> line1 >> line2;

  auto grid1 = pathToGrid(line1);
  auto grid2 = pathToGrid(line2);
  auto star1 =
      int(grid1.size() + grid2.size()); // larger than any possible result
  auto star2 =
      int(grid1.size() + grid2.size()); // larger than any possible result

  for (auto [pos, steps] : grid2) {
    if (grid1.count(pos) > 0) { // path intersection
      star1 = std::min(star1, abs(pos.first) + abs(pos.second));
      star2 = std::min(star2, grid1[pos] + grid2[pos]);
    }
  }

  std::cout << "Day 3 star 1 = " << star1 << "\n";
  std::cout << "Day 3 star 2 = " << star2 << "\n";
}
