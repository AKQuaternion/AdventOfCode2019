//
// Created by Chris Hartman on 12/18/19.
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

ifstream ifile("../day19.txt");
Intcode i(ifile);

int probe(int x, int y) {
  i.reset();
  auto s = i.run({x, y});
  auto out = i.getOutput();
  return out.front();
}

bool verify(int sx, int sy) {
  for (int x = 0; x < 100; ++x) {
    for (int y = 0; y < 100; ++y)
      if (probe(sx + x, sy + y) != 1)
        return false;
  }
  cout << sx * 10000 + sy << " is okay\n";
  return true;
}

void day19() {
  auto star1 = 0;
  auto star2 = 0;
  //
  //   for(int x=0;x<50;++x)
  //      for(int y=0;y<50;++y) {
  //       star1 += probe(x,y);
  //     }

  for (int r = 0; r < 110; ++r) {
    for (int c = 0; c < 110; ++c)
      if (c >= 86 && c < 86 + 10 && r >= 96 && r < 96 + 10)
        cout << '=';
      else if (probe(c, r))
        cout << 'X';
      else
        cout << '.';
    cout << endl;
  }
  int sx = 50;
  int sy = 50;
checkrow:
  for (int x = 0; x < 100; ++x) {
    if (probe(sx + x, sy) != 1) {
      ++sy;
      if (probe(sx, sy) != 1)
        ++sx;
      goto checkrow;
    }
  }
  for (int y = 0; y < 100; ++y) {
    if (probe(sx, sy + y) != 1) {
      ++sx;
      goto checkrow;
    }
  }
  verify(sx, sy);
  verify(sx - 1, sy);
  verify(sx, sy - 1);
  verify(sx - 1, sy - 1);
  cout << endl;
  cout << sx << " " << sy << endl;
  //    for(int y=0;y<50;++y) {
  //      auto s = i.run({x,y});
  //      auto out = i.getOutput();
  //      i.reset();
  //      star1 += out.front();
  //    }
  //

  //  string line;
  //  while (getline(ifile, line)) {
  //    string _s;
  //    istringstream iline(line);
  //    iline >> _s;
  //  }
  cout << "Day 19 star 1 = " << star1 << "\n";
  cout << "Day 19 star 2 = " << star2 << "\n";
}
