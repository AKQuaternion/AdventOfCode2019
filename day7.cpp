//
// Created by Chris Hartman on 12/6/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

auto runPhases(const Intcode &program, std::vector<int> phase, bool feedback) {
  long long star = 0;
  do {
    std::vector<Intcode> amps(5, program);
    for (int i = 0; i < 5; ++i)
      amps[i].enqueueInput({phase[i]});
    std::vector<long long> previousOutput({0});
    Intcode::State state{Intcode::HALT};
    do
      for (int i = 0; i < 5; ++i) {
        state = amps[i].run(previousOutput);
        previousOutput = amps[i].getOutput();
      }
    while (feedback && state != Intcode::HALT);
    star = std::max(star, previousOutput.back());
  } while (std::next_permutation(phase.begin(), phase.end()));
  return star;
}

void day7() {
  std::ifstream ifile("../day7.txt");
  Intcode program(ifile);
  auto star1 = runPhases(program, {0, 1, 2, 3, 4}, false);
  auto star2 = runPhases(program, {5, 6, 7, 8, 9}, true);
  std::cout << "Day 7 star 1 = " << star1 << "\n";
  std::cout << "Day 7 star 2 = " << star2 << "\n";
}