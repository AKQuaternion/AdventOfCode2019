//
// Created by Chris Hartman on 12/8/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
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

void day9() {
  ifstream ifile("../day9.txt");
  //  istringstream ifile("104,1125899906842624,99");
  //  istringstream
  //  ifile("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");
  //  istringstream ifile("1102,34915192,34915192,7,4,7,99,0");
  Intcode i(ifile);
  i.run({1});
  auto star1 = i.getOutput().back();
  i.reset();
  i.run({2});
  auto star2 = i.getOutput().back();
  cout << "Day 9 star 1 = " << star1 << "\n";
  cout << "Day 9 star 2 = " << star2 << "\n";
}