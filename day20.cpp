#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Day20 {
  using Vertex = std::pair<int, int>;
  using RVertex = std::pair<Vertex, int>; // Vertex and recursion level
public:
  Day20(const std::string &filename) {
    std::ifstream ifile(filename);
    std::string line;
    while (getline(ifile, line)) {
      grid.push_back(line);
    }

    findPortals();
    makeEdges();
  }
  int star1() { return distanceOfPath(false); }
  int star2() { return distanceOfPath(true); }

private:
  bool onOutside(int r, int c) {
    return (r == 2 || r == grid.size() - 3 || c == 2 || c == grid.size() - 3);
  }

  void findPortals() {
    for (int r = 0; r < grid.size(); ++r)
      for (int c = 0; c < grid[r].size(); ++c) {
        auto ch = grid[r][c];
        if (isupper(ch)) {
          std::string name;
          name += ch;
          if (r + 1 < grid.size() && isupper(grid[r + 1][c])) {
            name += grid[r + 1][c];
            if (r + 2 < grid.size() && grid[r + 2][c] == '.')
              portals[name].push_back({r + 2, c});
            else
              portals[name].push_back({r - 1, c});
          } else if (c + 1 < grid[r].size() && isupper(grid[r][c + 1])) {
            name += grid[r][c + 1];
            if (c + 2 < grid[r].size() && grid[r][c + 2] == '.')
              portals[name].push_back({r, c + 2});
            else
              portals[name].push_back({r, c - 1});
          }
        }
      }
  }

  void makeEdges() {
    for (int r = 0; r < grid.size(); ++r)
      for (int c = 0; c < grid[r].size(); ++c) {
        if (grid[r][c] != '.')
          continue;
        if (grid[r - 1][c] == '.')
          edges[{r, c}].push_back({{r - 1, c}, 0});
        if (grid[r + 1][c] == '.')
          edges[{r, c}].push_back({{r + 1, c}, 0});
        if (grid[r][c - 1] == '.')
          edges[{r, c}].push_back({{r, c - 1}, 0});
        if (grid[r][c + 1] == '.')
          edges[{r, c}].push_back({{r, c + 1}, 0});
      }

    for (auto [name, locations] : portals) {
      if (name == "AA" || name == "ZZ")
        continue;
      auto [r, c] = locations.front();
      int dLevel = onOutside(r, c) ? -1 : 1;
      edges[locations.front()].push_back({locations.back(), dLevel});
      edges[locations.back()].push_back({locations.front(), -dLevel});
    }
  }

  int distanceOfPath(bool recursiveMaze) {
    std::set<RVertex> visited;
    std::queue<std::pair<RVertex, int>> q; // node, distance
    q.push({RVertex{portals["AA"].front(), 0}, 0});
    while (!q.empty()) {
      auto [node, dist] = q.front();
      q.pop();
      if (visited.count(node))
        continue;
      if (node == RVertex{portals["ZZ"].front(), 0})
        return dist;
      visited.insert(node);
      auto [nodePos, nodeLvl] = node;
      for (auto [nbr, dLevel] : edges.at(nodePos)) {
        auto newLevel = recursiveMaze ? nodeLvl + dLevel : 0;
        if (!visited.count({nbr, newLevel}) && newLevel >= 0)
          q.push({{nbr, newLevel}, dist + 1});
      }
    }
    return -1;
  }

  std::vector<std::string> grid;
  std::map<Vertex, std::vector<RVertex>> edges;
  std::map<std::string, std::vector<Vertex>> portals;
};

void day20() {
  Day20 day20("../day20.txt");
  std::cout << "Day 20 star 1 = " << day20.star1() << "\n";
  std::cout << "Day 20 star 2 = " << day20.star2() << "\n";
}
