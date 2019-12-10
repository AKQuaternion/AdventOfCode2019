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

void day10() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day10.txt");
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
  //  Intcode i(ifile);
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
        cout << l << endl;
      }
      cout << endl;
      //      if(sum>star1)
      cout << col << " " << row << " has " << sum << endl;
      star1 = max(star1, sum);
    }
  cout << "Day 10 star 1 = " << star1 << "\n";
  cout << "Day 10 star 2 = " << star2 << "\n";
} // not 325

// day 1 calculate fuel required
// day 2 run IntCode program
// day 3 wires on a grid U D L R
// day 4 digit passwords
// day 5 more IntCode
// day 6 orbits tree traversal
// day 7 chained IntCode
// day 8 pixel image
// day 9