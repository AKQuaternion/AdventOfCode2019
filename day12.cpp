//
// Created by Chris Hartman on 12/11/19.
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
using std::ostream;
using std::pair;
using std::queue;
using std::set;
using std::sqrt;
using std::string;
using std::swap;
using std::tie;
using std::tuple;
using std::vector;

class Moon {
  friend ostream &operator<<(ostream &os, const Moon &m);
  friend void updateVelocity(Moon &m1, Moon &m2);

public:
  Moon(const string &s) {
    istringstream in(s);
    char c; // dummy
    in >> c >> c >> c >> p[0] >> c >> c >> c >> p[1] >> c >> c >> c >> p[2];
  }
  void updatePosition() {
    for (int ii = 0; ii < 3; ++ii)
      p[ii] += v[ii];
  }
  long long energy() {
    return (abs(p[0]) + abs(p[1]) + abs(p[2])) *
           (abs(v[0]) + abs(v[1]) + abs(v[2]));
  }

  // private:
  long p[3];
  long v[3] = {0, 0, 0};
};

ostream &operator<<(ostream &os, const Moon &m) {
  return os << "pos=<x=" << m.p[0] << ", y=" << m.p[1] << ", z=" << m.p[2]
            << ">, vel=<x=" << m.v[0] << ", y=" << m.v[1] << ", z=" << m.v[2]
            << ">";
}

void updateVelocity(Moon &m1, Moon &m2) {
  for (int ii = 0; ii < 3; ++ii)
    if (m1.p[ii] < m2.p[ii]) {
      ++m1.v[ii];
      --m2.v[ii];
    } else if (m1.p[ii] > m2.p[ii]) {
      --m1.v[ii];
      ++m2.v[ii];
    }
}

long long cycleTime(vector<int> p) {
  vector<int> v(p.size());
  auto orig = make_pair(p, v);
  for (int t = 1;; ++t) {
    //    cout << t;
    //    for(auto x:p)
    //      cout << " " << x;
    //    for(auto x: v)
    //      cout << " " << x;
    //    cout << endl;
    for (int p1 = 0; p1 < v.size(); ++p1)
      for (int p2 = p1 + 1; p2 < v.size(); ++p2)
        if (p[p1] < p[p2]) {
          ++v[p1];
          --v[p2];
        } else if (p[p1] > p[p2]) {
          --v[p1];
          ++v[p2];
        }
    for (int ii = 0; ii < p.size(); ++ii)
      p[ii] += v[ii];
    if (orig == make_pair(p, v))
      return t;
  }
}

void day12() {
  ifstream ifile("../day12.txt");
  //  istringstream ifile("<x=-1, y=0, z=2>\n"
  //                      "<x=2, y=-10, z=-7>\n"
  //                      "<x=4, y=-8, z=8>\n"
  //                      "<x=3, y=5, z=-1>");
  //    istringstream ifile("<x=-8, y=-10, z=0>\n"
  //                        "<x=5, y=5, z=10>\n"
  //                        "<x=2, y=-7, z=3>\n"
  //                        "<x=9, y=-8, z=-3>");
  string line;
  vector<Moon> moons;
  while (getline(ifile, line)) {
    moons.emplace_back(line);
  }

  long long star2 = 1;
  for (int ii = 0; ii < 3; ++ii) {
    vector<int> p;
    for (auto &m : moons)
      p.push_back(m.p[ii]);
    star2 = std::lcm(star2, cycleTime(p));
  }

  for (int time = 0; time < 1000; ++time) {
    //    cout << "After " << time << " steps:\n";
    //    for (auto &m : moons)
    //      cout << m << endl;
    //
    //    cout << endl;

    for (int m1 = 0; m1 < moons.size(); ++m1)
      for (int m2 = m1 + 1; m2 < moons.size(); ++m2)
        updateVelocity(moons[m1], moons[m2]);

    for (auto &m : moons)
      m.updatePosition();
  }

  auto star1 =
      std::accumulate(moons.begin(), moons.end(), 0,
                      [](auto sum, auto m) { return sum + m.energy(); });

  cout << "Day 12 star 1 = " << star1 << "\n";
  cout << "Day 12 star 2 = " << star2 << "\n";
}