//
// Created by Chris Hartman on 12/2/19.
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

void day3() {
  auto star1 = 999999999;
  auto star2 = 0;
  ifstream ifile("../day3.txt");
  //  Intcode    prog(ifile);
  // istringstream ifile("R75,D30,R83,U83,L12,D49,R71,U7,L72\n"
  //                      "U62,R66,U55,R34,D71,R55,D58,R83");
  string line;
  getline(ifile, line);
  string line2;
  getline(ifile, line2);
  istringstream in1(line);
  map<pair<int, int>, int> m;
  auto x = 0;
  auto y = 0;
  auto steps = 0;
  while (in1) {
    string s;
    char c;
    char _c;
    int i;
    //    istringstream iline(line);
    in1 >> c >> i >> _c;
    cout << c << i << endl;
    switch (c) {
    case 'U':
      for (int n = 0; n < i; ++n) {
        ++y;
        m[{x, y}] = 1;
      }
      break;
    case 'D':
      for (int n = 0; n < i; ++n) {
        --y;
        m[{x, y}] = 1;
      }
      break;
    case 'L':
      for (int n = 0; n < i; ++n) {
        --x;
        m[{x, y}] = 1;
      }
      break;
    case 'R':
      for (int n = 0; n < i; ++n) {
        ++x;
        m[{x, y}] = 1;
      }
      break;
    }
    cout << x << " " << y << endl;
  }
  istringstream in2(line2);
  cout << "------\n";
  x = y = 0;
  while (in2) {

    string s;
    char c;
    char _c;
    int i;
    //    istringstream iline(line);
    in2 >> c >> i >> _c;
    cout << c << i << endl;
    switch (c) {
    case 'U':
      for (int n = 0; n < i; ++n) {
        ++y;
        if (m[{x, y}] == 1)
          star1 = min(star1, abs(x) + abs(y));
      }
      break;
    case 'D':
      for (int n = 0; n < i; ++n) {
        --y;
        if (m[{x, y}] == 1)
          star1 = min(star1, abs(x) + abs(y));
      }
      break;
    case 'L':
      for (int n = 0; n < i; ++n) {
        --x;
        if (m[{x, y}] == 1)
          star1 = min(star1, abs(x) + abs(y));
      }
      break;
    case 'R':
      for (int n = 0; n < i; ++n) {
        ++x;
        if (m[{x, y}] == 1)
          star1 = min(star1, abs(x) + abs(y));
      }
      break;
    }
    cout << x << " " << y << endl;
  }

  cout << "Day 3 star 1 = " << star1 << "\n";
  cout << "Day 3 star 2 = " << star2 << "\n";
}

// day 1 calculate fuel required
// day 2 run IntCode program
