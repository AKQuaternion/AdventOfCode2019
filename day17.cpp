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
  auto out = i.getOutput();
  string asciiOutput;
  std::copy(out.begin(), out.end(), std::back_inserter(asciiOutput));
  cout << asciiOutput;
  istringstream fromOutput(asciiOutput);
  string line;
  vector<string> grid;
  while (getline(fromOutput, line)) {
    grid.push_back(line);
  }

  for (int r = 1; r < grid.size() - 1; ++r)
    for (int c = 1; c < grid[0].size() - 1; ++c) {
      if (grid[r][c] == '#' && grid[r + 1][c] == '#' && grid[r - 1][c] == '#' &&
          grid[r][c + 1] == '#' && grid[r][c - 1] == '#')
        star1 += r * c;
    }
  cout << "Day 17 star 1 = " << star1 << "\n";

  vector<pair<int, int>> dirs = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  string dirChars("v<^>"); // SWNE

  int r = 0;
  int c;
  while (true) {
    c = grid[r].find_first_of(dirChars);
    if (c != string::npos)
      break;
    ++r;
    assert(r < grid.size());
  }
  int droidDir = dirChars.find(grid[r][c]);

  auto left = [&](int dir) { return (dir + 3) % 4; };
  auto right = [&](int dir) { return (dir + 1) % 4; };
  auto onGrid = [&](int r, int c) {
    return r >= 0 && r < grid.size() && c >= 0 && c < grid[0].size();
  };
  auto scaffoldIsTo = [&](int dir) {
    auto qc = c + dirs[dir].first;
    auto qr = r + dirs[dir].second;
    return onGrid(qr, qc) && grid[qr][qc] == '#';
  };

  string path;
  while (true) { // invariant: no scaffold in front of us
    assert(!scaffoldIsTo(droidDir));
    if (scaffoldIsTo(left(droidDir))) {
      path += "L,";
      droidDir = left(droidDir);
    } else if (scaffoldIsTo(right(droidDir))) {
      path += "R,";
      droidDir = right(droidDir);
    } else
      break; // end of path
    int length = 0;
    while (scaffoldIsTo(droidDir)) {
      ++length;
      c += dirs[droidDir].first;
      r += dirs[droidDir].second;
    }
    path += std::to_string(length) + ",";
  }
  cout << path << endl;
  i.reset();
  i.freeplay();

  string walk = "A,A,B,C,B,C,B,C,B,A\n"
                "L,10,L,8,R,8,L,8,R,6\n"
                "R,6,R,8,R,8\n"
                "R,6,R,6,L,8,L,10\n"
                "n\n";
  vector<long long> hisin(walk.size());
  std::copy(walk.begin(), walk.end(), hisin.begin());
  i.enqueueInput(hisin);
  i.run();
  out = i.getOutput();
  cout << "Day 17 star 2 = " << out.back() << "\n";
}
