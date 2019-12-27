#include "Intcode.hpp"
#include <fstream>

void day9() {
  std::ifstream ifile("../day9.txt");
  Intcode i(ifile);
  i.run({1});
  auto star1 = i.getOutput().back();
  i.reset();
  i.run({2});
  auto star2 = i.getOutput().back();
  std::cout << "Day 9 star 1 = " << star1 << "\n";
  std::cout << "Day 9 star 2 = " << star2 << "\n";
}