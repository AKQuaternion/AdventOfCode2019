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
map<char, string> forFree;
set<Vertex> visited;
char maxKey = 'a';

void dfs(Vertex node, string &doors, string &keys, string &all) {
  visited.insert(node);
  auto ch = grid[node.first][node.second];
  if (islower(ch)) {
    keys += ch;
    all += ch;
    //    cout << "A: " << all << endl;
    mustBefore[ch] = doors;
    forFree[ch] = keys;
    maxKey = max(maxKey, ch);
  }
  if (isupper(ch)) {
    doors += tolower(ch);
    all += ch;
    //    cout << "A: " << all << endl;
  }
  for (auto nbr : edges[node]) {
    if (visited.count(nbr) == 0) {
      dfs(nbr, doors, keys, all);
    }
  }
  if (isupper(ch)) {
    doors.pop_back();
    all.pop_back();
  }
  if (islower(ch)) {
    keys.pop_back();
    all.pop_back();
  }
}

// map<char, map<char, int>> distances;
int distances[27][27];
int minDistance[26];

map<char, set<Vertex>> visitedFrom;

void dfsDistances(char from, Vertex node, int dist) {
  visitedFrom[from].insert(node);
  auto ch = grid[node.first][node.second];
  if (islower(ch)) {
    distances[from == '@' ? 0 : from - 'a' + 1][ch - 'a'] = dist;
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
      found[k - 'a'] = true;
      for (auto l : mustBefore[k])
        if (!found[l - 'a'])
          goto NEXTPERM;
    }
    cout << keys << endl;
  NEXTPERM:;
  } while (next_permutation(keys.begin(), keys.end()));
}

auto numOrderings = 0ull;
auto numBest = 0;
string bestOrder;
int bestFar = 999999999;

vector<vector<map<vector<int>, int>>> memos(26,
                                            vector<map<vector<int>, int>>(26));

int backtrack(string &order, vector<bool> &found, int howFar) {
  ++numOrderings;
  //  if (order.size() == maxKey - 'a' + 1) {
  if (std::all_of(found.begin(), found.end(), [](auto x) { return x; })) {
    //    cout << order << endl;
    if (howFar == bestFar)
      ++numBest;

    else if (howFar < bestFar) {
      numBest = 1;
      bestFar = howFar;
      bestOrder = order;
      cout << order << " with cost " << howFar << " after exploring "
           << numOrderings << endl;
    }
    return 0;
  }

  vector<int> nextChoice;
  for (int i = 0; i < found.size(); ++i) {
    if (found[i])
      continue;
    for (auto l : mustBefore[i + 'a'])
      if (!found[l - 'a'])
        goto NEXTCHOICE;
    nextChoice.push_back(i);
  NEXTCHOICE:;
  }

  if (!order.empty() &&
      memos[nextChoice.size()][order.back() - 'a'].count(nextChoice)) {
    auto newHowFar =
        howFar + memos[nextChoice.size()][order.back() - 'a'][nextChoice];
    if (newHowFar == bestFar)
      ++numBest;

    else if (newHowFar < bestFar) {
      numBest = 1;
      bestFar = newHowFar;
      bestOrder = order;
      cout << order << " with cost " << newHowFar << " after exploring "
           << numOrderings << endl;
    }
    return memos[nextChoice.size()][order.back() - 'a'][nextChoice];
  }

  auto dHowFar = 0;

  sort(nextChoice.begin(), nextChoice.end(), [&order](auto x, auto y) {
    auto firstCoord = (order.empty() ? 0 : order.back() - 'a' + 1);
    return distances[firstCoord][x] < distances[firstCoord][y];
  });

  int bestExtension = 999999999;
  auto bound = 0;
  for (auto i : nextChoice)
    bound += minDistance[i - 'a'];
  for (auto i : nextChoice) {
    auto oldFound = found;
    for (auto x : forFree.at(i + 'a'))
      found[x - 'a'] = true;
    auto thisStep = (order.empty() ? distances[0][i]
                                   : distances[order.back() - 'a' + 1][i]);
    auto newHowFar = howFar + thisStep;
    order.push_back('a' + i);
    assert(bound - minDistance[order.back() - 'a'] >= 0);
    int extension = bestExtension;
    if (newHowFar + bound - minDistance[order.back() - 'a'] < bestFar)
      //    if(newHowFar < bestFar)
      extension = thisStep + backtrack(order, found, newHowFar);
    order.pop_back();
    found = oldFound;
    if (extension < bestExtension)
      bestExtension = extension;
  }
  if (!order.empty())
    memos[nextChoice.size()][order.back() - 'a'][nextChoice] = bestExtension;
  return bestExtension;
}

void tryLast(char last) {
  string order;
  order.push_back(last);
  while (!mustBefore[last].empty()) {
    last = mustBefore[last].back();
    order.push_back(last);
  }
  cout << order << endl;
  int recent = 0;
  int howFar = 0;
  for (auto i = order.rbegin(); i != order.rend(); ++i) {
    //    cout << char(recent+'a'-1) << " to " << *i << " is " <<
    //    distances[recent][*i-'a'] << endl;
    howFar += distances[recent][*i - 'a'];
    recent = *i - 'a' + 1;
  }
  if (howFar > bestFar) {
    bestFar = howFar;
    bestOrder = order;
    cout << order << " with cost " << howFar << endl;
  }
}
} // namespace

map<char, Vertex> positions;

void day18() {
  auto star1 = 0;
  auto star2 = 0;
  //            ifstream ifile("../day19.txt");
  //  istringstream ifile("########################\n"
  //                      "#f.D.E.e.C.b.A.@.a.B.c.#\n"
  //                      "######################.#\n"
  //                      "#d.....................#\n"
  //                      "########################");
  //
  //    istringstream ifile("########################\n"
  //                        "#...............b.C.D.f#\n"
  //                        "#.######################\n"
  //                        "#.....@.a.B.c.d.A.e.F.g#\n"
  //                        "########################");
  //
  //    istringstream ifile("########################\n"
  //                        "#@..............ac.GI.b#\n"
  //                        "###d#e#f################\n"
  //                        "###A#B#C################\n"
  //                        "###g#h#i################\n"
  //                        "########################");

  istringstream ifile("#################\n"
                      "#i.G..c...e..H.p#\n"
                      "########.########\n"
                      "#j.A..b...f..D.o#\n"
                      "########@########\n"
                      "#k.E..a...g..B.n#\n"
                      "########.########\n"
                      "#l.F..d...h..C.m#\n"
                      "#################");
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
  string keys;
  string all{"@"};
  dfs(positions['@'], doors, keys, all);
  cout << "Max key is " << maxKey << endl;
  //  for (auto [c, s] : mustBefore)
  //    cout << c << " after " << s << endl;
  //  for (auto [c, s] : forFree)
  //    cout << c << " forFree gives " << s << endl;
  for (auto [c, pos] : positions) {
    dfsDistances(c, pos, 0);
  }

  for (auto c = 'a'; c <= maxKey; ++c)
    minDistance[c - 'a'] = *std::min_element(std::begin(distances[c - 'a' + 1]),
                                             std::end(distances[c - 'a' + 1]));

  //  for (auto [from, dists] : distances) {
  //    cout << from << " --> ";
  //    for (auto [to, d] : dists)
  //      cout << to << "=" << d << ", ";
  //    cout << endl;
  //  }
  //  findOrderings();

  string order;
  vector<bool> found(maxKey - 'a' + 1);
  star1 = backtrack(order, found, 0);
  cout << numOrderings << " nodes explored.\n";
  cout << numBest << " equivalent paths.\n";
  cout << "Best order was " << bestOrder << endl;
  bestFar = 0;

  //  for (auto last = 'a'; last <= maxKey; ++last)
  //    tryLast(last);
  cout << "Day 18 star 1 = " << star1 << "\n";
  cout << "Day 18 star 2 = " << star2 << "\n";
}
// xkvncotfhaegqyzbusdiwpjlrm with cost 5070 after exploring 27
// xkvncotfhaegqybzus with cost 4842 after exploring 64
// xkvncofhaegqybzuts with cost 4830 after exploring 87677
// xkvncohaegqybzutfs with cost 4814 after exploring 167418
// xkvncohaegqybzuts with cost 4802 after exploring 167420
// xkvnchaegqybzuotfs with cost 4670 after exploring 884616
// xkvnchaegqybzuots with cost 4658 after exploring 884618
// xkvnhaegqybzucotfs with cost 4574 after exploring 5969542
// xkvnhaegqybzucots with cost 4562 after exploring 5969544
// 5089249869
// Day 18 star 1 = 4562
// Day 18 star 2 = 0
// Time required: 3177.78 seconds
