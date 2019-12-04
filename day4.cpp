//
// Created by Chris Hartman on 12/3/19.
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

void day4() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day4.txt");
  Intcode i(ifile);
  string line;
  while (getline(ifile, line)) {
    string _s;
    istringstream iline(line);
    iline >> _s;
  }

  cout << "Day 4 star 1 = " << star1 << "\n";
  cout << "Day 4 star 2 = " << star2 << "\n";
}
