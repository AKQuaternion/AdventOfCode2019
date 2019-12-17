//
// Created by Chris Hartman on 12/16/19.
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

void day17() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day17.txt");
  Intcode i(ifile);
  i.run();
  string input(10000, '\0');
  auto out = i.getOutput();
  std::copy(out.begin(), out.end(), input.begin());
  cout << input << endl;
  istringstream iii(input);
  string line;
  vector<string> grid;
  while (getline(iii, line)) {
    grid.push_back(line);
  }

  for (int r = 1; r < grid.size() - 1; ++r)
    for (int c = 1; c < grid.size() - 1; ++c) {
      if (grid[r][c] == '#' && grid[r + 1][c] == '#' && grid[r - 1][c] == '#' &&
          grid[r][c + 1] == '#' && grid[r][c - 1] == '#')
        star1 += r * c;
    }
  cout << "Day 17 star 1 = " << star1 << "\n";
  cout << "Day 17 star 2 = " << star2 << "\n";
}
