//
// Created by Chris Hartman on 12/5/19.
//
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

class Graph {
public:
  Graph(std::istream &ifile) {
    std::string line;
    while (getline(ifile, line)) {
      std::istringstream iline(line);
      std::string from, to;
      getline(iline, from, ')');
      getline(iline, to);
      _edges[from].push_back(to);
      _edges[to].push_back(from);
    }
  }

  void setDistancesFrom(const std::string &root) {
    _distances.clear();
    traverse(root, 0);
  }

  [[nodiscard]] int sumOfDistances() const {
    return std::accumulate(_distances.begin(), _distances.end(), 0,
                           [](auto s, auto p) { return s + p.second; });
  }

  [[nodiscard]] int distanceTo(const std::string &node) const {
    return _distances.at(node);
  }

private:
  void traverse(const std::string &node, int depth) {
    _distances[node] = depth;
    for (const auto &s : _edges[node])
      if (_distances.count(s) == 0)
        traverse(s, depth + 1);
  }

  std::map<std::string, std::vector<std::string>> _edges;
  std::map<std::string, int> _distances;
};

void day6() {
  std::ifstream ifile("../day6.txt");
  Graph orbits(ifile);

  orbits.setDistancesFrom("COM");
  auto star1 = orbits.sumOfDistances();

  orbits.setDistancesFrom("YOU");
  auto star2 = orbits.distanceTo("SAN") - 2;
  std::cout << "Day 6 star 1 = " << star1 << "\n";
  std::cout << "Day 6 star 2 = " << star2 << "\n";
}