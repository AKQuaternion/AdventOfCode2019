//
// Created by Chris Hartman on 12/22/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

void day23() {
  auto star1 = 0;
  auto star2 = 0;
  std::ifstream ifile("../day23.txt");
  Intcode i(ifile);

  std::vector<Intcode> nic(50, i);
  for (int j = 0; j < 50; ++j)
    nic[j].enqueueInput({j});

  long long natX = 0, natY = 0, lastNatY = 0;
  while (star2 == 0) {
    for (auto &c : nic) {
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
  std::cout << "Day 23 star 1 = " << star1 << "\n";
  std::cout << "Day 23 star 2 = " << star2 << "\n";
}
