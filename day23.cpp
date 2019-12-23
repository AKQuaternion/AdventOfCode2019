//
// Created by Chris Hartman on 12/22/19.
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

void day23() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day23.txt");
  Intcode i(ifile);

  vector<Intcode> nic(50, i);
  for (int j = 0; j < 50; ++j)
    nic[j].enqueueInput({j});

  long long natX = 0, natY = 0, lastNatY = 0;
  while (star2 == 0) {
    for (int x = 0; x < 50; ++x) {
      auto &c = nic[x];
      c.step();
      if (c.getOutputSize() == 3) {
        auto out = c.getOutput();
        if (out[0] == 255) {
          natX = out[1];
          natY = out[2];
          if (lastNatY == natY)
            star2 = natY;
          lastNatY = natY;
          if (star1 == 0)
            star1 = out[2];
        } else {
          nic[out[0]].enqueueInput({out[1], out[2]});
        }
      }
    }
    if (std::all_of(nic.begin(), nic.end(), [](auto &c) { return c.isIdle(); }))
      nic[0].enqueueInput({natX, natY});
  }
  cout << "Day 23 star 1 = " << star1 << "\n";
  cout << "Day 23 star 2 = " << star2 << "\n";
}
