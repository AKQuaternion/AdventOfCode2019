#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <utility>
#include <vector>

using std::deque;
using std::pair;
using std::string;
using std::vector;

bool onGrid(const vector<string> &g, int y, int x) {
  return (y >= 0 && y < g.size() && x >= 0 && x < g.size());
}

// fire returns the position of asteroid hit when firing a laser from bc, br
// in the direction dx, dy, or returns {-1,-1} if no asteroid is hit
std::optional<pair<int, int>> fire(const vector<string> &g, int bc, int br,
                                   int dx, int dy) {
  for (int t = 1; true; ++t)
    if (onGrid(g, br + t * dy, bc + t * dx)) {
      if (g[br + t * dy][bc + t * dx] == '#')
        return {{bc + t * dx, br + t * dy}};
    } else
      return {};
}

vector<int> d10old;

void day10old() {
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
          return fire(grid, x, y, s.first, s.second);
        });
        if (detected > star1) {
          star1 = detected;
          bestCol = x;
          bestRow = y;
        }
      }

  int count = 0;
  while (star2 == 0)
    for (auto [dx, dy] : slopes)
      if (auto hitPos = fire(grid, bestCol, bestRow, dx, dy)) {
        grid[hitPos->second][hitPos->first] = ' ';
        d10old.push_back(hitPos->first * 100 + hitPos->second);
        if (++count == 200)
          star2 = hitPos->first * 100 + hitPos->second;
      }

  std::cout << "Day 10 star 1 = " << star1 << "\n";
  std::cout << "Day 10 star 2 = " << star2 << "\n";
}

bool byatan2(pair<int, int> s1, pair<int, int> s2) {
  return atan2(s1.first, s1.second) > atan2(s2.first, s2.second);
};
//
// void day10() {
//  day10old();
//  auto star1 = 0, star2 = 0;
//  std::ifstream ifile("../day10.txt");
//  string line;
//
//  vector<pair<int, int>> asteroids;
//  for (int row = 0; getline(ifile, line); ++row)
//    for (int col = 0; col < line.size(); ++col)
//      if(line[col]=='#')
//        asteroids.emplace_back(col, row);
//
//  map<pair<int, int>, deque<int>, decltype(byatan2) *>
//      gcdsBySlopeForBestAsteroid(byatan2);
//
//  int bestX = 0, bestY = 0;
//  for (auto [ax, ay] : asteroids) {
//    map<pair<int, int>, deque<int>, decltype(byatan2) *> gcdsBySlope(byatan2);
//    for (auto [bx, by] : asteroids) {
//      if (bx == ax && by == ay)
//        continue;
//      auto dx = bx - ax;
//      auto dy = by - ay;
//      auto g = std::abs(std::gcd(dx, dy));
//      gcdsBySlope[{dx / g, dy / g}].push_back(g);
//    }
//    if (gcdsBySlope.size() > star1) {
//      star1 = gcdsBySlope.size();
//      bestX = ax;
//      bestY = ay;
//      gcdsBySlopeForBestAsteroid = gcdsBySlope;
//    }
//  }
//
//  for (auto &[pos, qOfGs] : gcdsBySlopeForBestAsteroid)
//    std::sort(qOfGs.begin(), qOfGs.end());
//
//  vector<pair<int, int>> destructionOrder;
//  while (destructionOrder.size() < 200) {
//    for (auto &[pos, qOfGs] : gcdsBySlopeForBestAsteroid) {
//      if (qOfGs.empty())
//        continue;
//      auto [dx, dy] = pos;
//      auto g = qOfGs.front();
//      qOfGs.pop_front();
//      destructionOrder.emplace_back((bestX + g * dx)*100+ bestY + g * dy);
//    }
//  }
//  auto [it1,it2] =
//  std::mismatch(d10old.begin(),d10old.end(),destructionOrder.begin(),destructionOrder.end());
//
////  for(int
///Number=(it2-destructionOrder.begin());Number<destructionOrder.size();++Number)
////    std::cout << Number << " " <<
/// destructionOrder[Number].first*100+destructionOrder[Number].second << "\n";
//  auto [x, y] = destructionOrder[200];
//  std::cout << "Day 10 star 1 = " << star1 << "\n";
//  std::cout << "Day 10 star 2 = " << x * 100 + y << "\n";
//}
