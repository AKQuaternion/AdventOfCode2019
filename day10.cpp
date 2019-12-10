//
// Created by Chris Hartman on 12/9/19.
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

namespace {
auto star1 = 0;
auto star2 = 0;
} // namespace

bool fire(vector<string> &g, int bc, int br, int dx, int dy, int &count) {
  int s = g.size() - 1;
  for (int m = 1; m < g.size(); ++m)
    if (bc + m * dx < 0 || bc + m * dx > s || br + m * dy < 0 ||
        br + m * dy > s)
      return false;
    else {
      if (g[br + m * dy][bc + m * dx] == '#') {
        ++count;
        //        cout << "Slope " << dy << "/" << dx << "\t" << bc + m * dx <<
        //        " " << br + m * dy << " at count "
        //             << count << endl;
        //        g[br + m * dy][bc + m * dx] = 'O';
        //        for (auto l : g) {
        //          cout << l << endl;
        //        }
        //        cout << endl;
        if (count == 200) {
          star2 = (bc + m * dx) * 100 + br + m * dy;
          //          exit(0);
        }
        g[br + m * dy][bc + m * dx] = ' ';
        return true;
      }
    }
  return false;
}

void day10() {
  int br, bc;
  //  istringstream ifile(".#..#..###\n"
  //                      "####.###.#\n"
  //                      "....###.#.\n"
  //                      "..###.##.#\n"
  //                      "##.##.#.#.\n"
  //                      "....###..#\n"
  //                      "..#.#..#.#\n"
  //                      "#..#.#.###\n"
  //                      ".##...##.#\n"
  //                      ".....#.#..");
  //  istringstream ifile(".#..#\n"
  //                      ".....\n"
  //                      "#####\n"
  //                      "....#\n"
  //                      "...##");
  // istringstream ifile("......#.#.\n"
  //                      "#..#.#....\n"
  //                      "..#######.\n"
  //                      ".#.#.###..\n"
  //                      ".#..#.....\n"
  //                      "..#....#.#\n"
  //                      "#..#....#.\n"
  //                      ".##.#..###\n"
  //                      "##...#..#.\n"
  //                      ".#....####");

  istringstream ifilet(".#..##.###...#######\n"
                       "##.############..##.\n"
                       ".#.######.########.#\n"
                       ".###.#######.####.#.\n"
                       "#####.##.#.##.###.##\n"
                       "..#####..#.#########\n"
                       "####################\n"
                       "#.####....###.#.#.##\n"
                       "##.#################\n"
                       "#####.##.###..####..\n"
                       "..######..##.#######\n"
                       "####.##.####...##..#\n"
                       ".#####..#.######.###\n"
                       "##...#.##########...\n"
                       "#.##########.#######\n"
                       ".####.#.###.###.#.##\n"
                       "....##.##.###..#####\n"
                       ".#.#.###########.###\n"
                       "#.#.#.#####.####.###\n"
                       "###.##.####.##.#..##");
  ifstream ifile("../day10.txt");
  string line;
  vector<string> grid;
  while (getline(ifile, line)) {
    grid.push_back(line);
  }

  assert(grid.size() == grid[0].size());
  int s = grid.size() - 1;
  for (int row = 0; row < grid.size(); ++row)
    for (int col = 0; col < grid.size(); ++col) {
      if (grid[row][col] == '.')
        continue;
      auto g = grid;
      for (int dy = -s; dy <= s; ++dy)
        for (int dx = -s; dx <= s; ++dx) {
          if (dx == 0 && dy == 0)
            continue;
          if (row + dy > s || row + dy < 0)
            continue;
          if (col + dx > s || col + dx < 0)
            continue;
          bool clear = true;
          for (int m = 1; m < s; m++) {
            //              if(row==2&&col==0&&dy==0&&dx==1&&m==1)
            //                ++star2;
            if (row + m * dy > s || row + m * dy < 0)
              continue;
            if (col + m * dx > s || col + m * dx < 0)
              continue;
            if (g[row + m * dy][col + m * dx] == '#') {
              if (clear)
                clear = false;
              else
                g[row + m * dy][col + m * dx] = '*';
            }
          }
        }
      g[row][col] = '+';
      int sum = 0;
      for (auto l : g) {
        sum += count(l.begin(), l.end(), '#');
        //        cout << l << endl;
      }
      //      cout << endl;
      if (sum > star1) {
        //        cout << col << " " << row << " has " << sum << endl;
        br = row;
        bc = col;
      }
      star1 = max(star1, sum);
    }
  int sum = 0;
  for (auto l : grid) {
    sum += count(l.begin(), l.end(), '#');
    //        cout << l << endl;
  }
  cout << sum << " stars in grid.\n";

  cout << "Day 10 star 1 = " << star1 << "\n";
  cout << " at " << bc << " " << br << endl;

  auto g = grid;
  g[br][bc] = 'o';

  vector<pair<int, int>> slopes;
  for (int dx = 1; dx < s; ++dx)
    for (int dy = 1; dy < s; ++dy)
      if (std::gcd(dx, dy) == 1)
        slopes.push_back({dx, dy});

  sort(slopes.begin(), slopes.end(), [](auto a, auto b) {
    auto sa = double(a.second) / a.first;
    auto sb = double(b.second) / b.first;
    return sa < sb;
  });

  int count = 0;

  for (int i = 0; i < 100; ++i) {
    fire(g, bc, br, 0, -1, count);
    for (auto [rise, run] : slopes)
      fire(g, bc, br, run, -rise, count);
    fire(g, bc, br, 1, 0, count);
    std::reverse(slopes.begin(), slopes.end());
    for (auto [rise, run] : slopes)
      fire(g, bc, br, run, rise, count);
    fire(g, bc, br, 0, 1, count);
    std::reverse(slopes.begin(), slopes.end());
    for (auto [rise, run] : slopes)
      fire(g, bc, br, -run, rise, count);
    fire(g, bc, br, -1, 0, count);
    std::reverse(slopes.begin(), slopes.end());
    for (auto [rise, run] : slopes)
      fire(g, bc, br, -run, -rise, count);
    std::reverse(slopes.begin(), slopes.end());
  }

  cout << "Day 10 star 2 = " << star2 << "\n";
}

// day 1 calculate fuel required
// day 2 run IntCode program
// day 3 wires on a grid U D L R
// day 4 digit passwords
// day 5 more IntCode
// day 6 orbits tree traversal
// day 7 chained IntCode
// day 8 pixel image
// day 9