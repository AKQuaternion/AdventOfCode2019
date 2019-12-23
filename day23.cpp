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

  long long natX, natY;
  long long lastYsent = -1;
  while (true) {
    bool allIdle = true;
    for (int x = 0; x < 50; ++x) {
      auto &c = nic[x];
      if (c.getInputSize() == 0)
        c.enqueueInput({-1});
      else
        allIdle = false;
      auto s = c.run();
      assert(s == Intcode::INPUT);
      if (c.probeOutput() == 255 && c.getOutputSize() >= 2) {
        auto out = c.getOutput();
        for (auto o : out)
          cout << o << " ";
        cout << endl;
        int outP = 0;
        while (outP < out.size()) {
          natX = out[outP + 1];
          natY = out[outP + 2];
          outP += 3;
        }
        //        return;
      }
      if (c.getOutputSize() >= 3) {
        auto out = c.getOutput();
        int outP = 0;
        while (outP < out.size()) {
          nic[out[outP]].enqueueInput({out[outP + 1], out[outP + 2]});
          //          cout << out[outP] << " <-- " << out[outP + 1] << "," <<
          //          out[outP + 2]
          //               << endl;
          outP += 3;
        }
      }
    }
    if (allIdle) {
      nic[0].enqueueInput({natX, natY});
      if (lastYsent == natY) {
        cout << "!!! " << natY;
        return;
      }
      lastYsent = natY;
    }
  }
  cout << "Day 23 star 1 = " << star1 << "\n";
  cout << "Day 23 star 2 = " << star2 << "\n";
}
