#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <utility>
#include <vector>

struct CompareByatan2 {
  bool operator()(std::pair<int, int> s1, std::pair<int, int> s2) const {
    return atan2(s1.first, s1.second) > atan2(s2.first, s2.second);
  }
};

void day10() {
  std::ifstream ifile("../day10.txt");
  std::string line;

  std::vector<std::pair<int, int>> asteroids;
  for (int row = 0; getline(ifile, line); ++row)
    for (int col = 0; col < line.size(); ++col)
      if (line[col] == '#')
        asteroids.emplace_back(col, row);

  std::map<std::pair<int, int>, std::vector<int>, CompareByatan2>
      gcdsBySlopeForBestAsteroid;

  int bestX = 0, bestY = 0;
  auto star1 = 0;
  for (auto [ax, ay] : asteroids) {
    std::map<std::pair<int, int>, std::vector<int>, CompareByatan2> gcdsBySlope;
    for (auto [bx, by] : asteroids) {
      if (bx == ax && by == ay)
        continue;
      auto dx = bx - ax;
      auto dy = by - ay;
      auto g = std::abs(std::gcd(dx, dy));
      gcdsBySlope[{dx / g, dy / g}].push_back(g);
    }
    if (gcdsBySlope.size() > star1) {
      star1 = gcdsBySlope.size();
      bestX = ax;
      bestY = ay;
      gcdsBySlopeForBestAsteroid = gcdsBySlope;
    }
  }

  for (auto &[pos, qOfGs] : gcdsBySlopeForBestAsteroid)
    std::sort(qOfGs.begin(), qOfGs.end(), std::greater<>());

  std::vector<int> destructionOrder;
  while (destructionOrder.size() < 200) {
    for (auto &[pos, qOfGs] : gcdsBySlopeForBestAsteroid) {
      if (qOfGs.empty())
        continue;
      auto [dx, dy] = pos;
      auto g = qOfGs.back();
      qOfGs.pop_back();
      destructionOrder.emplace_back((bestX + g * dx) * 100 + bestY + g * dy);
    }
  }
  auto star2 = destructionOrder.at(199);
  std::cout << "Day 10 star 1 = " << star1 << "\n";
  std::cout << "Day 10 star 2 = " << star2 << "\n";
}