//
// Created by Chris Hartman on 12/6/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

void day7() {
  long long star1 = 0;
  long long star2 = 0;
  ifstream ifile("../day7.txt");
  Intcode program(ifile);

  vector<long long> phase{0, 1, 2, 3, 4};
  do {
    vector<Intcode> amps(5, program);
    vector<vector<long long>> inputs;
    for (auto p : phase)
      inputs.push_back({p});
    vector<long long> out{0};
    for (int i = 0; i < 5; ++i) {
      inputs[i].push_back(out[0]);
      out = amps[i].run(inputs[i]);
      inputs[i].clear();
    }
    star1 = std::max(star1, out[0]);
  } while (std::next_permutation(phase.begin(), phase.end()));

  phase = {5, 6, 7, 8, 9};
  do {
    vector<Intcode> amps(5, program);
    vector<vector<long long>> inputs;
    for (auto p : phase)
      inputs.push_back({p});
    vector<long long> out{0};
    while (out.size() != 2) { //!!! hack if Intcode halts it returns two output
      auto last = out[0];
      for (int i = 0; i < 5; ++i) {
        inputs[i].push_back(out[0]);
        out = amps[i].run(inputs[i]);
        inputs[i].clear();
      }
      star2 = std::max(star2, last);
    }
  } while (std::next_permutation(phase.begin(), phase.end()));

  cout << "Day 7 star 1 = " << star1 << "\n";
  cout << "Day 7 star 2 = " << star2 << "\n";
}
// Day 7 star 1 = 273814
// Day 7 star 2 = 34579864