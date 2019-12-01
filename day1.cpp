//
// Created by Chris Hartman on 12/1/19.
//
#include <fstream>
#include <iostream>
#include <numeric>
using std::cout;
using std::endl;
using std::ifstream;

void day1() {
  auto star1 = 0;
  auto star2 = 0;

  ifstream ifile("../day1.txt");
  int fuel;
  while (ifile >> fuel) {
    star1 += fuel / 3 - 2;
    while ((fuel = fuel / 3 - 2) >= 0)
      star2 += fuel;
  }

  cout << "Day 1 star 1 = " << star1 << "\n";
  cout << "Day 1 star 2 = " << star2 << "\n";
}
// Day 1 star 1 = 3372756
// Day 1 star 2 = 5056279
