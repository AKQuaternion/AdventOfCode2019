//
// Created by Chris Hartman on 12/6/19.
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

void day7() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day7.txt");
  Intcode i(ifile);
  //  string line;
  //  while (getline(ifile, line)) {
  //    string _s;
  //    istringstream iline(line);
  //    iline >> _s;
  //  }

  vector<int> phase{0, 1, 2, 3, 4};
  do {
    for (auto i : phase)
      cout << i;
    cout << endl;

    Intcode a{i}, b{i}, c{i}, d{i}, e{i};

    vector<int> out;
    out = a.run({phase[0], 0});
    out = b.run({phase[1], out[0]});
    out = c.run({phase[2], out[0]});
    out = d.run({phase[3], out[0]});
    out = e.run({phase[4], out[0]});
    star1 = max(star1, out[0]);
  } while (std::next_permutation(phase.begin(), phase.end()));

  cout << "Day 7 star 1 = " << star1 << "\n";
  cout << "Day 7 star 2 = " << star2 << "\n";
}