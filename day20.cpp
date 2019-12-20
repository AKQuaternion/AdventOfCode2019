//
// Created by Chris Hartman on 12/19/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
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
using std::pair;
using std::queue;
using std::set;
using std::string;
using std::tuple;
using std::vector;

using Vertex = pair<int, int>;
using RVertex = pair<Vertex, int>;
map<Vertex, vector<RVertex>> edges;
map<string, vector<Vertex>> portals;

auto findChoices() {
  std::ofstream out("out.txt");
  set<RVertex> visited;
  map<RVertex, RVertex> parent;
  queue<pair<RVertex, int>> q; // node, distance
  q.push({{portals["AA"].front(), 0}, 0});
  while (!q.empty()) {
    auto [node, dist] = q.front();
    auto node2 = node;
    q.pop();
    if (node == RVertex{portals["ZZ"].front(), 0}) {
      cout << dist << endl;
      break;
    }
    if (visited.count(node))
      continue;           // don't visit a node we've already seen
    visited.insert(node); // mark this node as visited
    auto [loc, lvl] = node;
    auto loc2 = loc;
    auto lvl2 = lvl;
    for (auto [nbr, dLevel] : edges[loc]) {
      auto nbr2 = nbr;
      auto dLevel2 = dLevel;
      if (lvl + dLevel >= 0 && !visited.count({nbr, lvl + dLevel})) {
        q.push({{nbr, lvl + dLevel}, dist + 1}); // search neighbor nodes
        parent[{nbr, lvl + dLevel}] = node;
        //        out << "(" << loc.first << "," << loc.second << ")" << lvl <<
        //        " ----> "; out << "(" << nbr.first << "," << nbr.second << ")"
        //        << lvl + dLevel
        //            << endl;
      }
    }
  }

  auto node = RVertex{portals["ZZ"].front(), 0};
  while (node != RVertex{portals["AA"].front(), 0}) {
    auto [loc, lvl] = node;
    out << "(" << loc.first << "," << loc.second << ")" << lvl << endl;
    node = parent[node];
  }
}

void day20() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day20.txt");
  //    istringstream ifile("         A           \n"
  //                        "         A           \n"
  //                        "  #######.#########  \n"
  //                        "  #######.........#  \n"
  //                        "  #######.#######.#  \n"
  //                        "  #######.#######.#  \n"
  //                        "  #######.#######.#  \n"
  //                        "  #####  B    ###.#  \n"
  //                        "BC...##  C    ###.#  \n"
  //                        "  ##.##       ###.#  \n"
  //                        "  ##...DE  F  ###.#  \n"
  //                        "  #####    G  ###.#  \n"
  //                        "  #########.#####.#  \n"
  //                        "DE..#######...###.#  \n"
  //                        "  #.#########.###.#  \n"
  //                        "FG..#########.....#  \n"
  //                        "  ###########.#####  \n"
  //                        "             Z       \n"
  //                        "             Z       ");

  //  istringstream ifile("             Z L X W       C                 \n"
  //                      "             Z P Q B       K                 \n"
  //                      "  ###########.#.#.#.#######.###############  \n"
  //                      "  #...#.......#.#.......#.#.......#.#.#...#  \n"
  //                      "  ###.#.#.#.#.#.#.#.###.#.#.#######.#.#.###  \n"
  //                      "  #.#...#.#.#...#.#.#...#...#...#.#.......#  \n"
  //                      "  #.###.#######.###.###.#.###.###.#.#######  \n"
  //                      "  #...#.......#.#...#...#.............#...#  \n"
  //                      "  #.#########.#######.#.#######.#######.###  \n"
  //                      "  #...#.#    F       R I       Z    #.#.#.#  \n"
  //                      "  #.###.#    D       E C       H    #.#.#.#  \n"
  //                      "  #.#...#                           #...#.#  \n"
  //                      "  #.###.#                           #.###.#  \n"
  //                      "  #.#....OA                       WB..#.#..ZH\n"
  //                      "  #.###.#                           #.#.#.#  \n"
  //                      "CJ......#                           #.....#  \n"
  //                      "  #######                           #######  \n"
  //                      "  #.#....CK                         #......IC\n"
  //                      "  #.###.#                           #.###.#  \n"
  //                      "  #.....#                           #...#.#  \n"
  //                      "  ###.###                           #.#.#.#  \n"
  //                      "XF....#.#                         RF..#.#.#  \n"
  //                      "  #####.#                           #######  \n"
  //                      "  #......CJ                       NM..#...#  \n"
  //                      "  ###.#.#                           #.###.#  \n"
  //                      "RE....#.#                           #......RF\n"
  //                      "  ###.###        X   X       L      #.#.#.#  \n"
  //                      "  #.....#        F   Q       P      #.#.#.#  \n"
  //                      "  ###.###########.###.#######.#########.###  \n"
  //                      "  #.....#...#.....#.......#...#.....#.#...#  \n"
  //                      "  #####.#.###.#######.#######.###.###.#.#.#  \n"
  //                      "  #.......#.......#.#.#.#.#...#...#...#.#.#  \n"
  //                      "  #####.###.#####.#.#.#.#.###.###.#.###.###  \n"
  //                      "  #.......#.....#.#...#...............#...#  \n"
  //                      "  #############.#.#.###.###################  \n"
  //                      "               A O F   N                     \n"
  //                      "               A A D   M                     ");

  int outC, inC, outR, inR;

  string line;
  vector<string> grid;
  while (getline(ifile, line)) {
    grid.push_back(line);
  }

  for (int r = 0; r < grid.size(); ++r)
    for (int c = 0; c < grid[r].size(); ++c) {
      auto ch = grid[r][c];
      if (ch != '.')
        continue;
      auto putEdge = [&](int y, int x) {
        if (grid[y][x] == '.')
          edges[{r, c}].push_back({{y, x}, 0});
      };
      putEdge(r - 1, c);
      putEdge(r + 1, c);
      putEdge(r, c + 1);
      putEdge(r, c - 1);
    }

  for (int r = 0; r < grid.size(); ++r)
    for (int c = 0; c < grid[r].size(); ++c) {
      auto ch = grid[r][c];
      if (isupper(ch)) {
        string name;
        name += ch;
        if (r + 1 < grid.size() && isupper(grid[r + 1][c])) {
          name += grid[r + 1][c];
          //            cout << name;
          if (r + 2 < grid.size() && grid[r + 2][c] == '.') {

            portals[name].push_back({r + 2, c});
            cout << r + 2 << "," << c << endl;
          } else {
            portals[name].push_back({r - 1, c});
            cout << r - 1 << "," << c << endl;
          }
        } else if (c + 1 < grid[r].size() && isupper(grid[r][c + 1])) {
          name += grid[r][c + 1];
          cout << name;
          if (c + 2 < grid[r].size() && grid[r][c + 2] == '.') {
            portals[name].push_back({r, c + 2});
            cout << r << "," << c + 2 << endl;
          } else {
            portals[name].push_back({r, c - 1});
            cout << r << "," << c - 1 << endl;
          }
        }
      }
    }

  for (auto [name, vs] : portals) {
    if (name == "AA" || name == "ZZ")
      continue;
    int first;
    {
      auto [r1, c1] = vs.front();
      int dLevel = 0;
      if (r1 == 2 || r1 == grid.size() - 3 || c1 == 2 ||
          c1 == grid[0].size() - 3)
        dLevel = -1; // outer to inner
      else
        dLevel = 1;
      edges[vs.front()].push_back({vs.back(), dLevel});
      first = dLevel;
    }
    {
      auto [r1, c1] = vs.back();
      int dLevel = 0;
      if (r1 == 2 || r1 == grid.size() - 3 || c1 == 2 ||
          c1 == grid[0].size() - 3)
        dLevel = -1; // outer to inner
      else
        dLevel = 1;
      edges[vs.back()].push_back({vs.front(), dLevel});
      assert(first != dLevel);
    }
  }

  for (auto [p, es] : edges) {
    cout << p.first << "," << p.second << " --> ";
    for (auto [e, lvl] : es)
      cout << e.first << "," << e.second << "(" << lvl << ")   ";
    cout << endl;
  }

  for (auto [name, es] : portals) {
    if (name == "AA" || name == "ZZ")
      continue;
    cout << name << " --> ";
    for (auto e : es)
      cout << e.first << "," << e.second << "   ";
    cout << endl;
  }

  findChoices();

  cout << grid.size() << " " << grid[0].size();
  cout << "Day 20 star 1 = " << star1 << "\n";
  cout << "Day 20 star 2 = " << star2 << "\n";
} // not 1306
