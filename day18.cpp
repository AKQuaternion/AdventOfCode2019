//
// Created by Chris Hartman on 12/17/19.
//
#include "Intcode.hpp"

#include <cctype>

#include <algorithm>
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
using std::copy;
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

namespace {
vector<string> grid;

using Vertex = pair<int, int>;

map<Vertex, vector<Vertex>> edges;
map<char, string> mustBefore;
set<Vertex> visited;
char maxKey = 'a';

void dfs(Vertex node, string &doors) {
  visited.insert(node);
  auto ch = grid[node.first][node.second];
  if (islower(ch)) {
    mustBefore[ch] += doors;
    maxKey = max(maxKey, ch);
  }
  if (isupper(ch))
    doors += tolower(ch);
  for (auto nbr : edges[node]) {
    if (visited.count(nbr) == 0) {
      dfs(nbr, doors);
    }
  }
  if (isupper(ch))
    doors.pop_back();
}

map<char, map<char, int>> distances;
int distances[27][27];
map<char, set<Vertex>> visitedFrom;

void dfsDistances(char from, Vertex node, int dist) {
  visitedFrom[from].insert(node);
  auto ch = grid[node.first][node.second];
  if (islower(ch)) {
    distances[from][ch] = dist;
  }

  for (auto nbr : edges[node]) {
    if (visitedFrom[from].count(nbr) == 0) {
      dfsDistances(from, nbr, dist + 1);
    }
  }
}

void findOrderings() {
  string keys(maxKey - 'a' + 1, ' ');
  std::iota(keys.begin(), keys.end(), 'a');
  do {
    vector<int> found(keys.size());
    for (auto k : keys) {
      found[k - 'a'] = 1;
      for (auto l : mustBefore[k])
        if (!found[l - 'a'])
          goto NEXTPERM;
    }
    cout << keys << endl;
  NEXTPERM:;
  } while (next_permutation(keys.begin(), keys.end()));
}

auto numOrderings = 0ull;
string bestOrder;
int bestFar = 999999999;
void backtrack(string &order, vector<int> &found, int howFar) {
  if (order.size() == maxKey - 'a' + 1) {
    //    cout << order << endl;
    if (howFar < bestFar) {
      bestFar = howFar;
      bestOrder = order;
      cout << order << " with cost " << howFar << endl;
    }
    ++numOrderings;
    return;
  }
  for (int i = 0; i < found.size(); ++i) {
    if (found[i])
      continue;
    for (auto l : mustBefore[i + 'a'])
      if (!found[l - 'a'])
        goto NEXTCHOICE;
    {
      found[i] = 1;
      auto newHowFar =
          howFar + (order.empty() ? distances['@']['a' + i]
                                  : distances[order.back()]['a' + i]);
      order.push_back('a' + i);
      backtrack(order, found, newHowFar);
      order.pop_back();
      found[i] = 0;
    }
  NEXTCHOICE:;
  }
}
} // namespace

map<char, Vertex> positions;

void day18() {
  auto star1 = 0;
  auto star2 = 0;
  //    ifstream ifile("../day19.txt");
  //  istringstream ifile("########################\n"
  //                      "#f.D.E.e.C.b.A.@.a.B.c.#\n"
  //                      "######################.#\n"
  //                      "#d.....................#\n"
  //                      "########################");

  istringstream ifile("########################\n"
                      "#...............b.C.D.f#\n"
                      "#.######################\n"
                      "#.....@.a.B.c.d.A.e.F.g#\n"
                      "########################");
  string line;

  while (getline(ifile, line)) {
    grid.push_back(line);
  }

  for (int r = 0; r < grid.size(); ++r)
    for (int c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == '#')
        continue;
      if (grid[r][c] != '.' && !isupper(grid[r][c]))
        positions[grid[r][c]] = {r, c};
      auto putEdge = [&](int y, int x) {
        if (grid[y][x] != '#')
          edges[{r, c}].push_back({y, x});
      };
      putEdge(r - 1, c);
      putEdge(r + 1, c);
      putEdge(r, c + 1);
      putEdge(r, c - 1);
    }

  //  for(auto [e,v]: edges) {
  //    cout << e.first << "," << e.second << " --> ";
  //    for(auto [y,x]:v)
  //      cout << y << "," << x << " ";
  //    cout << endl;
  //  }
  string doors;
  dfs(positions['@'], doors);
  cout << "Max key is " << maxKey << endl;
  for (auto [c, s] : mustBefore)
    cout << c << " after " << s << endl;

  for (auto [c, pos] : positions) {
    dfsDistances(c, pos, 0);
  }

  //  for (auto [from, dists] : distances) {
  //    cout << from << " --> ";
  //    for (auto [to, d] : dists)
  //      cout << to << "=" << d << ", ";
  //    cout << endl;
  //  }
  //  findOrderings();

  string order;
  vector<int> found(maxKey - 'a' + 1);
  backtrack(order, found, 0);
  cout << numOrderings << endl;
  cout << "Day 18 star 1 = " << star1 << "\n";
  cout << "Day 18 star 2 = " << star2 << "\n";
}
