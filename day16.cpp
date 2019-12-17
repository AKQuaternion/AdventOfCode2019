//
// Created by Chris Hartman on 12/15/19.
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

int digitsToNum(int numDigits, const vector<int> digits) {
  int num = 0;
  for (int i = 0; i < numDigits; ++i)
    num = 10 * num + digits[i];
  return num;
}

void day16() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day16.txt");
  string line;

  ifile >> line;
  vector<int> original;
  for (auto c : line)
    original.push_back(c - '0');

  auto offset = digitsToNum(7, original);
  cout << "Offset is " << offset << endl;

  auto v = original;

  int sequence[] = {0, 1, 0, -1};
  for (int t = 1; t <= 100; ++t) {
    vector<int> n;
    for (int i = 0; i < v.size(); ++i) {
      int sum = 0;
      for (int j = 0; j < v.size(); ++j) {
        sum += (sequence[((j + 1) % (4 * (i + 1))) / (i + 1)] * v[j]);
      }
      n.push_back(abs(sum) % 10);
    }
    v = n;
  }

  cout << "Day 16 star 1 = " << digitsToNum(8, v) << "\n";

  v.resize(10000 * original.size() - offset);
  for (int s = 0; s < v.size(); ++s)
    v[s] = original[(s + offset) % original.size()];

  for (int i = 0; i < 100; ++i)
    std::partial_sum(v.crbegin(), v.crend(), v.rbegin(),
                     [](int sum, int digit) { return (sum + digit) % 10; });

  cout << "Day 16 star 2 = " << digitsToNum(8, v) << "\n";
}
