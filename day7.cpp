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

int runPhases(const Intcode &program, vector<int> phase, bool feedback) {
  int star = 0;
  do {
    vector<Intcode> amps(5, program);
    for (int i = 0; i < 5; ++i)
      amps[i].enqueueInput({phase[i]});
    int previousOutput = 0;
    Intcode::State state;
    do
      for (int i = 0; i < 5; ++i)
        std::tie(state, previousOutput) = amps[i].run({previousOutput});
    while (feedback && state != Intcode::HALT);
    star = std::max(star, previousOutput);
  } while (std::next_permutation(phase.begin(), phase.end()));
  return star;
}

void day7() {
  ifstream ifile("../day7.txt");
  Intcode program(ifile);
  auto star1 = runPhases(program, {0, 1, 2, 3, 4}, false);
  auto star2 = runPhases(program, {5, 6, 7, 8, 9}, true);
  cout << "Day 7 star 1 = " << star1 << "\n";
  cout << "Day 7 star 2 = " << star2 << "\n";
}
// Day 7 star 1 = 273814
// Day 7 star 2 = 34579864