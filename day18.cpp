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
    mustBefore[ch] = doors + keys;
    keys += ch;
    all += ch;
    //    cout << "A: " << all << endl;
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
    distances[from == '@' ? 0 : from - 'a' + 1][ch - 'a'] =
        (from == ch ? 9999 : dist);
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

pair<bool, int> backtrack(string &order, vector<bool> &found, int howFar) {
  cout << order << endl;
  ++numOrderings;
  if (numOrderings % 100'000'000 == 0)
    cout << numOrderings << " nodes processed, order is " << order << endl;
  if (order.size() == maxKey - 'a' + 1) {
    //  if (std::all_of(found.begin(), found.end(), [](auto x) { return x; })) {
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
    return {false, 0};
  }
  //  if(order == "acfidg")
  //    cout << " w";
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
    return {false, memos[nextChoice.size()][order.back() - 'a'][nextChoice]};
  }

  if (order.size() <= 3)
    sort(nextChoice.begin(), nextChoice.end(), [&order](auto x, auto y) {
      auto firstCoord = (order.empty() ? 0 : order.back() - 'a' + 1);
      return distances[firstCoord][x] < distances[firstCoord][y];
    });

  if (order.empty()) {
    for (auto n : nextChoice)
      cout << char(n + 'a');
    cout << " <--- search order for first key\n";
  }
  int bestExtension = 999999999;

  auto bound = 0;
  auto bound2 = 0;
  //  if(order == "agbfceidhmlknj")
  //    cout << "break\n";
  if (!order.empty() && found.size() - order.size() >= 35) {
    auto j = order.back();
    auto min1 = 9999;
    auto min2 = 9999;
    for (int i = 0; i < found.size(); ++i) {
      if (found[i])
        continue;
      auto dist = distances[j - 'a' + 1][i];
      if (dist < min1) {
        min2 = min1;
        min1 = dist;
      } else if (dist < min2)
        min2 = dist;
    }
    bound = min1 - 2 * min2;
    //    cout << "+" << min1 << " -2*" << min2 << endl;
    for (int j2 = 0; j2 < found.size(); ++j2) {
      if (found[j2])
        continue;
      min1 = 9999;
      min2 = 9999;
      for (int i = 0; i < found.size(); ++i) {
        if (found[i] && i != order.back() - 'a')
          continue;
        auto dist = distances[j2 + 1][i];
        if (dist < min1) {
          min2 = min1;
          min1 = dist;
        } else if (dist < min2)
          min2 = dist;
      }
      //      cout << "+" << min1 << " +" << min2 << endl;
      assert(min2 < 9999);
      bound += min1 + min2;
    }
    bound /= 2;
    //    cout << "/2 = " << bound << endl;
  }
  //  else if (!order.empty()) {
  //    for (int j = 0; j < found.size(); ++j) {
  //      if (found[j])
  //        continue;
  //      auto minDistToj = 999999;
  //      for (int i = 0; i < found.size(); ++i) {
  //        if (i == j)
  //          continue;
  //        if (!found[i] || (order.back() == i + 'a')) {
  //          minDistToj = min(minDistToj, distances[i + 1][j]);
  //        }
  //      }
  //      bound2 += minDistToj;
  //      //      cout << "+" << minDistToj << endl;
  //    }
  //  }
  //  cout << "=" << bound2 << endl;
  //      assert(bound >= bound2);
  bound = max(bound, bound2);

  if (howFar + bound > bestFar)
    return {true, bestExtension};

  bool wePruned = false;
  for (auto i : nextChoice) {
    //    auto oldFound = found;
    //    for (auto x : forFree.at(i + 'a'))
    //      found[x - 'a'] = true;
    found[i] = true;
    auto thisStep = (order.empty() ? distances[0][i]
                                   : distances[order.back() - 'a' + 1][i]);
    auto newHowFar = howFar + thisStep;
    order.push_back('a' + i);

    auto [theyPruned, extension] = backtrack(order, found, newHowFar);
    extension += thisStep;
    wePruned |= theyPruned;
    order.pop_back();
    //    found = oldFound;
    found[i] = false;
    if (extension < bestExtension)
      bestExtension = extension;
  }
  if (!wePruned && !order.empty() && bestExtension < 999999999)
    memos[nextChoice.size()][order.back() - 'a'][nextChoice] = bestExtension;
  assert(bestExtension >= bound);
  return {wePruned, bestExtension};
}
} // namespace

map<char, Vertex> positions;

void day18() {
  auto star2 = 0;
  //    ifstream ifile("../day18.txt");
  //        istringstream ifile("########################\n"
  //                            "#f.D.E.e.C.b.A.@.a.B.c.#\n"
  //                            "######################.#\n"
  //                            "#d.....................#\n"
  //                            "########################");
  //
  //        istringstream ifile("########################\n"
  //                            "#...............b.C.D.f#\n"
  //                            "#.######################\n"
  //                            "#.....@.a.B.c.d.A.e.F.g#\n"
  //                            "########################");

  //      istringstream ifile("########################\n"
  //                          "#@..............ac.GI.b#\n"
  //                          "###d#e#f################\n"
  //                          "###A#B#C################\n"
  //                          "###g#h#i################\n"
  //                          "########################");

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
  //    for (auto [c, s] : forFree)
  //      cout << c << " forFree gives " << s << endl;
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
  auto [pruned, star1] = backtrack(order, found, 0);
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
// xkvncotfhaegqybzus with cost 4842 after exploring 67
// xkvncofhaegqybzuts with cost 4830 after exploring 282499
// xkvncohaegqybzutfs with cost 4814 after exploring 471861
// xkvncohaegqybzuts with cost 4802 after exploring 471863
// xkvnchaegqybzuotfs with cost 4670 after exploring 2901767
// xkvnchaegqybzuots with cost 4658 after exploring 2901769
// xkvnhaegqybzucotfs with cost 4574 after exploring 40560518
// xkvnhaegqybzucots with cost 4562 after exploring 40560520
// xvnhkaegqybzucots with cost 4558 after exploring 1280535894

// Without sorting:
// abcdefgh <--- search order for first key
// abcdefghijklmnop with cost 178 after exploring 17
// abcdefghijopknlm with cost 176 after exploring 327
// abcdefghipjoklmn with cost 174 after exploring 1027
// abcdefghipjoknlm with cost 172 after exploring 1035
// abcdefghklmnjoip with cost 170 after exploring 2338
// abcdefghlmknipjo with cost 168 after exploring 3490
// abcdefghlmknjoip with cost 166 after exploring 3500
// abcdefghmlknipjo with cost 164 after exploring 4247
// abcdefghmlknjoip with cost 162 after exploring 4257
// abcdefgnhmlkipjo with cost 160 after exploring 7787
// abcdefgnhmlkjoip with cost 158 after exploring 7797
// abcdefognhmlkipj with cost 156 after exploring 18807
// abcdefognhmlkjip with cost 154 after exploring 18810
// abcdfognhmepijkl with cost 152 after exploring 76321
// abcdfognhmlepijk with cost 150 after exploring 76388
// abcdgnhmepifojkl with cost 148 after exploring 107498
// abcdhmgnepifojkl with cost 144 after exploring 134429
// abcefdlhmgnkjoip with cost 142 after exploring 156826
// abdhgnciepfojklm with cost 140 after exploring 536267
// abjcefdlhmgnkoip with cost 138 after exploring 1348505
// abjdhgnciepfoklm with cost 136 after exploring 1409029
// 23157377 nodes explored.
// 1568 equivalent paths.
// Best order was abjdhgnciepfoklm
// Day 18 star 1 = 136
// Day 18 star 2 = 0
// Time required: 26.7047 seconds

// With sorting
// abfgcdeh <--- search order for first key
// agbfceidhmlknjop with cost 150 after exploring 17
// agbfceidlhmknjop with cost 146 after exploring 123
// agbfciedlhmknjop with cost 142 after exploring 10853
// agbjfciedlhmknop with cost 138 after exploring 97639
// agdhbjfociepknlm with cost 136 after exploring 311472
// 22365270 nodes explored.
// 1568 equivalent paths.
// Best order was agdhbjfociepknlm
// Day 18 star 1 = 136
// Day 18 star 2 = 0
// Time required: 28.337 seconds

// Max key is i
// defa <--- search order for first key
// defacigbh with cost 87 after exploring 10
// deacfi with cost 85 after exploring 20
// dacfi with cost 83 after exploring 74
// acfidgb with cost 81 after exploring 230
// 362 nodes explored.
// 2 equivalent paths.
// Best order was acfidgb
// Day 18 star 1 = 81
// Day 18 star 2 = 0
// Time required: 0.000322427 seconds