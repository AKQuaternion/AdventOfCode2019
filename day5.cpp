//
// Created by Chris Hartman on 12/4/19.
//
#include "Intcode.hpp"

#include <fstream>
#include <iostream>

void day5() {
  std::ifstream ifile("../day5.txt");
  Intcode i(ifile);
  i.run({1});
  auto star1 = i.getOutput().back();
  i.reset();
  i.run({5});
  auto star2 = i.getOutput().back();
  std::cout << "Day 5 star 1 = " << star1 << "\n";
  std::cout << "Day 5 star 2 = " << star2 << "\n";
}