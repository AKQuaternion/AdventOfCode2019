//
// Created by Chris Hartman on 12/12/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <curses.h>
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

void day13() {
  long score = 0;
  int ballPos = 0;
  int paddlePos = 0;
  char symbol[] = " |#=o";
//  initscr();
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day13.txt");
  Intcode i(ifile);
  i.freeplay();
  map<pair<int, int>, int> grid;
  vector<long long> input;
  auto dir = [](int b, int p) {
    if (b < p)
      return -1;
    if (b == p)
      return 0;
    return 1;
  };
  while (true) {
    auto [s, x] = i.run(input);
    input.clear();
    if (s == Intcode::HALT)
      break;
    if (s == Intcode::INPUT) {
      input = {dir(ballPos, paddlePos)};
      continue;
    }
    auto [s2, y] = i.run();
    auto [s3, z] = i.run();
    if (z == 4)
      ballPos = x;
    if (z == 3)
      paddlePos = x;
    if (x == -1 && y == 0) {
      score = z;
//      cout << "Score = " << score << "\n";
    }
//    clear();
//    for (auto row = 0; row < 25; ++row) {
//      for (auto col = 0; col < 80; ++col) {
//        move(row, col);
//        addch((symbol[grid[{col, row}]]));
//      }
//    }
//    move(0, 0);
//    std::ostringstream out;
//    out << score;
//    addstr(out.str().c_str());
//    refresh();
  }

  cout << "Day 13 star 1 = " << star1 << "\n";
  cout << "Day 13 star 2 = " << score << "\n";
}