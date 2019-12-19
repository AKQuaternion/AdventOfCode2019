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
map<char, Vertex> positionOf;
char maxKey = 'a';

auto bfs(string order) {
  vector<pair<char, int>> result;
  assert(!order.empty());
  auto ch = order.back();
  set<Vertex> visited;
  queue<pair<Vertex, int>> q;
  q.push({positionOf[ch], 0});
  while (!q.empty()) {
    //    auto [node, dist] = q.front();
    auto p = q.front();
    auto node = p.first;
    auto dist = p.second; // just for debugging
    q.pop();
    if (visited.count(node))
      continue;
    visited.insert(node);
    ch = grid[node.first][node.second];
    if (islower(ch) && order.find(ch) == string::npos) {
      result.emplace_back(ch, dist);
    }
    if (isupper(ch) && order.find(tolower(ch)) == string::npos)
      continue;
    for (auto nbr : edges[node])
      if (visited.count(nbr) == 0)
        q.push({nbr, dist + 1});
  }
  return result;
}

auto numNodesExplored = 0ull;
int bestFar = 999999999;

map<string, int> memos;
map<string, char> memoOrder;

int backtrack(string &order, int howFar) {
  ++numNodesExplored;

  auto memoString = order;
  if (!memoString.empty())
    sort(memoString.begin(), memoString.end() - 1);
  if (memos.count(memoString)) {
    auto newHowFar = howFar + memos.at(memoString);
    if (newHowFar < bestFar) {
      bestFar = newHowFar;
      cout << order << " with cost " << newHowFar << " after exploring "
           << numNodesExplored << endl;
    }
    return memos.at(memoString);
  }

  auto possibleNextChoices = bfs(order);

  if (numNodesExplored < 100 || order.size() <= 5) {
    cout << order << "    "; // << endl;
    for (auto [n, dist] : possibleNextChoices)
      cout << n << ":" << dist << ",  ";
    cout << endl;
  }

  int bestExtension = 0;
  char bestChar = '\0';
  bool first = true;

  for (auto [n, dist] : possibleNextChoices) {
    auto newHowFar = howFar + dist;
    order.push_back(n);

    auto extension = dist + backtrack(order, newHowFar);
    if (first || extension < bestExtension) {
      first = false;
      bestExtension = extension;
      bestChar = order.back();
    }
    order.pop_back();
  }
  if (first)
    if (howFar < bestFar) {
      bestFar = howFar;
      cout << order << " with cost " << howFar << " after exploring "
           << numNodesExplored << endl;
    }
  memos[memoString] = bestExtension;
  memoOrder[memoString] = bestChar;
  return bestExtension;
}
} // namespace

// int calculateDistance(const string &s) {
//  auto d = 0;
//  char last = 'a' - 1;
//  for (auto c : s) {
//    //    cout << last << " -> " << c << " = " << distances[last-'a'+1][c-'a']
//    //    << endl;
//    d += distances[last - 'a' + 1][c - 'a'];
//    last = c;
//  }
//  return d;
//}

void showDistance(string s) { // stop at null!!!!
  auto mutateOrder = s;
  while (true) {
    if (!mutateOrder.empty())
      sort(mutateOrder.begin(), mutateOrder.end() - 1);
    mutateOrder.push_back(memoOrder[mutateOrder]);
    if (mutateOrder.back() == '\0')
      break;
    s.push_back(mutateOrder.back());
  }
  //  cout << s << " has distance " << calculateDistance(s) << endl;
}

void day18() {
  auto star2 = 0;
  ifstream ifile("../day18.txt");
  //  istringstream ifile("########################\n"
  //                      "#f.D.E.e.C.b.A.@.a.B.c.#\n"
  //                      "######################.#\n"
  //                      "#d.....................#\n"
  //                      "########################");
  //
  //  istringstream ifile("########################\n"
  //                      "#...............b.C.D.f#\n"
  //                      "#.######################\n"
  //                      "#.....@.a.B.c.d.A.e.F.g#\n"
  //                      "########################");

  //        istringstream ifile("########################\n"
  //                            "#@..............ac.GI.b#\n"
  //                            "###d#e#f################\n"
  //                            "###A#B#C################\n"
  //                            "###g#h#i################\n"
  //                            "########################");

  //      istringstream ifile("#################\n"
  //                          "#i.G..c...e..H.p#\n"
  //                          "########.########\n"
  //                          "#j.A..b...f..D.o#\n"
  //                          "########@########\n"
  //                          "#k.E..a...g..B.n#\n"
  //                          "########.########\n"
  //                          "#l.F..d...h..C.m#\n"
  //                          "#################");
  string line;

  while (getline(ifile, line)) {
    grid.push_back(line);
  }

  for (int r = 0; r < grid.size(); ++r)
    for (int c = 0; c < grid[r].size(); ++c) {
      if (islower(grid[r][c]))
        maxKey = max(grid[r][c], maxKey);
      if (grid[r][c] == '#')
        continue;
      if (grid[r][c] != '.' && !isupper(grid[r][c])) // key or @
        positionOf[grid[r][c]] = {r, c};
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

  cout << "Max key is " << maxKey << endl;

  string order{"@"};
  auto star1 = backtrack(order, 0);
  cout << numNodesExplored << " nodes explored.\n";

  cout << "Day 18 star 1 = " << star1 << "\n";
  cout << "Day 18 star 2 = " << star2 << "\n";
  showDistance("@");
  //  showDistance("afbjgnhdloepcikm");
} // not 4530, not 4550
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