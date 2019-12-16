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

void day16() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day16.txt");
  string line;

  ifile >> line;
  cout << line.size() << endl;
  //  line = "12345678";
  // line = "03036732577212944063491565474664";
  vector<char> x(1);
  for (auto c : line)
    x.push_back(c - '0');

  //  vector<char> v(10000*x.size());
  //  auto spot = v.begin();
  //  for(int i=0;i<10000;++i)
  //    spot = std::copy(x.begin(),x.end(),spot);
  auto v = x;
  int sequence[] = {0, 1, 0, -1};
  for (int t = 1; t <= 100; ++t) {
    cout << t << endl;
    vector<char> n(1);
    for (int i = 1; i < v.size(); ++i) {
      int sum = 0;
      for (int j = 0; j < v.size(); ++j) {
        sum += (sequence[((j) % (4 * (i))) / (i)] * v[j]);
      }
      n.push_back(abs(sum) % 10);
      //      cout << " = " << int(n.back()) << endl;
    }
    v = n;
    for (auto c : v)
      cout << int(c) << " ";
    cout << endl;
  }
  //  for(int i=5970221;i<5970221+8;++i)
  //  for(int i=0303673;i<0303673+8;++i)
  for (int i = 1; i < 9; ++i)
    cout << int(v[i]);
  cout << endl;
  cout << "Day 16 star 1 = " << star1 << "\n";
  cout << "Day 16 star 2 = " << star2 << "\n";
}
// 44098263

// 1
// 4 8 2 2 6 1 5 8
// 2
// 3 4 0 4 0 4 3 8
// 3
// 0 3 4 1 5 5 1 8
// 4
// 0 1 0 2 9 4 9 8
// 5
// 0 2 1 4 0 1 7 8
// 6
// 8 5 5 2 6 6 5 8
// 7
// 4 1 3 9 5 9 3 8
