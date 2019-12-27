#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

class Moon {
  friend std::ostream &operator<<(std::ostream &os, const Moon &m);
  friend void updateVelocity(Moon &m1, Moon &m2);

public:
  explicit Moon(const std::string &s) {
    std::istringstream in(s);
    char c = ' '; // dummy
    in >> c >> c >> c >> p[0] >> c >> c >> c >> p[1] >> c >> c >> c >> p[2];
  }
  void updatePosition() {
    for (int ii = 0; ii < 3; ++ii)
      p[ii] += v[ii];
  }
  long energy() {
    return (abs(p[0]) + abs(p[1]) + abs(p[2])) *
           (abs(v[0]) + abs(v[1]) + abs(v[2]));
  }

  // private:
  long p[3]{};
  long v[3] = {0, 0, 0};
};

std::ostream &operator<<(std::ostream &os, const Moon &m) {
  return os << "pos=<x=" << m.p[0] << ", y=" << m.p[1] << ", z=" << m.p[2]
            << ">, vel=<x=" << m.v[0] << ", y=" << m.v[1] << ", z=" << m.v[2]
            << ">";
}

void updateVelocity(Moon &m1, Moon &m2) {
  for (int ii = 0; ii < 3; ++ii) // TODO !!! Factor out inner part of this loop
    if (m1.p[ii] < m2.p[ii]) {
      ++m1.v[ii];
      --m2.v[ii];
    } else if (m1.p[ii] > m2.p[ii]) {
      --m1.v[ii];
      ++m2.v[ii];
    }
}

long cycleTime(std::vector<long> p) {
  std::vector<int> v(p.size());
  auto orig = make_pair(p, v);
  for (int t = 1;; ++t) {
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
  std::ifstream ifile("../day12.txt");
  std::string line;
  std::vector<Moon> moons;
  while (getline(ifile, line)) {
    moons.emplace_back(line);
  }

  long long star2 = 1;
  for (int ii = 0; ii < 3; ++ii) {
    std::vector<long> p;
    for (auto &m : moons)
      p.push_back(m.p[ii]);
    star2 = std::lcm(star2, cycleTime(p));
  }

  for (int time = 0; time < 1000; ++time) {
    for (int m1 = 0; m1 < moons.size(); ++m1)
      for (int m2 = m1 + 1; m2 < moons.size(); ++m2)
        updateVelocity(moons[m1], moons[m2]);

    for (auto &m : moons)
      m.updatePosition();
  }

  auto star1 =
      std::accumulate(moons.begin(), moons.end(), 0,
                      [](auto sum, auto m) { return sum + m.energy(); });

  std::cout << "Day 12 star 1 = " << star1 << "\n";
  std::cout << "Day 12 star 2 = " << star2 << "\n";
}