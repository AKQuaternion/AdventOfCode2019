//
// Created by Chris Hartman on 12/1/19.
//
//
// Created by Chris Hartman on 12/1/19.
//
#include <fstream>
#include <iostream>

#include "Intcode.hpp"

void day2() {
  std::ifstream ifile("../day2.txt");
  Intcode program(ifile);
  auto star1 = program.run(12, 2);

  auto star2 = 0;
  for (int noun = 0; noun < 100; ++noun)
    for (int verb = 0; verb < 100; ++verb)
      if (program.run(noun, verb) == 19690720)
        star2 = noun * 100 + verb;

  std::cout << "Day 2 star 1 = " << star1 << "\n";
  std::cout << "Day 2 star 2 = " << star2 << "\n";
}
