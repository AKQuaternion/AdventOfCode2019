//
// Created by Chris Hartman on 12/4/19.
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

void day5() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day5.txt");
  Intcode i(ifile);
  star1 = i.run({1}).back();
  star2 = i.run({5}).back();
  cout << "Day 5 star 1 = " << star1 << "\n";
  cout << "Day 5 star 2 = " << star2 << "\n";
}

// day 1 calculate fuel required
// day 2 run IntCode program
// day 3 wires on a grid U D L R