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

  while (true) {
    for (int x = 0; x < 50; ++x) {
      auto &c = nic[x];
      c.step();
      if (c.probeOutput() == 255 && c.getOutputSize() >= 2) {
        auto out = c.getOutput();
        cout << out[1] << endl;
        if (out.size() > 2)
          cout << out[2] << endl;
        return;
      }
      if (c.getOutputSize() == 3) {
        auto out = c.getOutput();
        nic[out[0]].enqueueInput({out[1], out[2]});
      }
    }
  }
  cout << "Day 23 star 1 = " << star1 << "\n";
  cout << "Day 23 star 2 = " << star2 << "\n";
}
