//
// Created by Chris Hartman on 12/23/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
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

vector<string> lines;
int nbrs(int r, int c) {
  int n = 0;
  if (r > 0)
    n += lines[r - 1][c] == '#';
  if (r < 4)
    n += lines[r + 1][c] == '#';
  if (c > 0)
    n += lines[r][c - 1] == '#';
  if (c < 4)
    n += lines[r][c + 1] == '#';
  return n;
}

unsigned long diversity(vector<string> &lines) {
  unsigned long v = 1;
  auto total = 0ul;
  for (int r = 0; r < 5; ++r)
    for (int c = 0; c < 5; ++c) {
      if (lines[r][c] == '#')
        total += v;
      v <<= 1;
    }
  return total;
}
void day24() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day24.txt");

  //  istringstream ifile("....#\n"
  string line;

  while (getline(ifile, line)) {
    lines.push_back(line);
  }

  map<vector<string>, int> history;

  auto time = 0;
  history[lines] = time;

  for (auto l : lines)
    cout << l << endl;
  cout << endl;
  auto other = lines;
  while (true) {
    time++;
    for (int r = 0; r < 5; ++r)
      for (int c = 0; c < 5; ++c) {
        if (lines[r][c] == '#')
          other[r][c] = (nbrs(r, c) == 1) ? '#' : '.';
        else
          other[r][c] = (nbrs(r, c) >= 1 && nbrs(r, c) <= 2) ? '#' : '.';
      }
    lines = other;
    if (history.count(lines)) {
      cout << diversity(lines) << endl;
      break;
    }
    history[lines] = time;
    for (auto l : lines)
      cout << l << endl;
    cout << endl;
  }
  cout << "Day 24 star 1 = " << star1 << "\n";
  cout << "Day 24 star 2 = " << star2 << "\n";
} // not 33554431
  // not 31462453

// Each minute, The bugs live and die based on the number of bugs in the four
// adjacent tiles:
//
// A bug dies (becoming an empty space) unless there is exactly one bug adjacent
// to it. An empty space becomes infested with a bug if exactly one or two bugs
// are adjacent to it.
// Initial state:
//....#
//#..#.
//#..##
//..#..
//#....
//
// After 1 minute:
//#..#.
//####.
//###.#
//##.##
//.##..
//
// After 2 minutes:
//#####
//....#
//....#
//...#.
//#.###
//
// After 3 minutes:
//#....
//####.
//...##
//#.##.
//.##.#
//
// After 4 minutes:
//####.
//....#
//##..#
//.....
//##...