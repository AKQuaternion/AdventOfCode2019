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
  vector<char> x;
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
    vector<char> n;
    for (int i = 0; i < v.size(); ++i) {
      int sum = 0;
      for (int j = 0; j < v.size(); ++j) {
        sum += (sequence[((j + 1) % (4 * (i + 1))) / (i + 1)] * v[j]);
      }
      n.push_back(abs(sum) % 10);
      //      cout << " = " << int(n.back()) << endl;
      cout << int(n.back()) << " ";
    }
    v = n;
    cout << endl;
  }
  //  for(int i=5970221;i<5970221+8;++i)
  //  for(int i=0303673;i<0303673+8;++i)
  for (int i = 0; i < 8; ++i)
    cout << int(v[i]);
  cout << endl;
  cout << "Day 16 star 1 = " << star1 << "\n";
  cout << "Day 16 star 2 = " << star2 << "\n";
} // 23845678 no
// 44098263