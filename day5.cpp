//
// Created by Chris Hartman on 12/4/19.
//
#include "Intcode.hpp"

#include <fstream>
#include <iostream>

void day5() {
  std::ifstream ifile("../day5.txt");
  Intcode i(ifile);
  auto star1 = 0;
  while (star1 == 0)
    star1 = i.run({1}).second;
  i.reset();
  auto star2 = i.run({5}).second;
  std::cout << "Day 5 star 1 = " << star1 << "\n";
  std::cout << "Day 5 star 2 = " << star2 << "\n";
}