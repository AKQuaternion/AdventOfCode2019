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
map<Vertex, Vertex> seenBy;
set<Vertex> visited;
char maxKey = 'a';

void dfs(Vertex node, string &doors, string &keys, string &all) {
  //!!! Try passing by value, make sure still works
  auto ch = grid[node.first][node.second];
  if (islower(ch)) {
    mustBefore[ch] = doors + keys;
    keys += ch;
    all += ch;
    //    cout << "A: " << all << endl;
    maxKey = max(maxKey, ch);
  }
  if (isupper(ch)) {
    doors += tolower(ch);
    all += ch;
    //    cout << "A: " << all << endl;
  }
  for (auto nbr : edges[node]) {
    if (nbr == seenBy[node]) // should assert this happens, except for root
      continue;
    if (seenBy.count(nbr) != 0) {
      if (visited.count(nbr))
        cout << "Not a tree (but still a DAG" << endl;
      else
        cout << "Found a cycle, not a DAG!" << endl;
    } else {
      seenBy[nbr] = node;
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
  visited.insert(node);
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

auto numNodesExplored = 0ull;
int bestFar = 999999999;

map<string, int> memos;
map<string, char> memoOrder;

int backtrack(string &order, vector<bool> &found, int howFar) {
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

  vector<int> possibleNextChoices;
  for (int i = 0; i < found.size(); ++i) {
    if (found[i])
      continue;
    bool prereqs = true;
    for (auto l : mustBefore.at(i + 'a'))
      if (!found[l - 'a']) {
        prereqs = false;
        break;
      }
    if (prereqs)
      possibleNextChoices.push_back(i);
  }

  if (numNodesExplored < 100 || order.size() <= 5) {
    cout << order << "    "; // << endl;
    for (auto n : possibleNextChoices)
      cout << char('a' + n);
    cout << endl;
  }

  int bestExtension = 0;
  char bestChar = '\0';
  bool first = true;

  for (auto i : possibleNextChoices) {
    found[i] = true;
    auto thisStep = (order.empty() ? distances[0][i]
                                   : distances[order.back() - 'a' + 1][i]);
    auto newHowFar = howFar + thisStep;
    order.push_back('a' + i);

    auto extension = thisStep + backtrack(order, found, newHowFar);
    if (first || extension < bestExtension) {
      first = false;
      bestExtension = extension;
      bestChar = order.back();
    }
    order.pop_back();
    found[i] = false;
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

map<char, Vertex> positions;

int calculateDistance(const string &s) {
  auto d = 0;
  char last = 'a' - 1;
  for (auto c : s) {
    //    cout << last << " -> " << c << " = " << distances[last-'a'+1][c-'a']
    //    << endl;
    d += distances[last - 'a' + 1][c - 'a'];
    last = c;
  }
  return d;
}

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
  cout << s << " has distance " << calculateDistance(s) << endl;
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

  //    istringstream ifile("#################\n"
  //                        "#i.G..c...e..H.p#\n"
  //                        "########.########\n"
  //                        "#j.A..b...f..D.o#\n"
  //                        "########@########\n"
  //                        "#k.E..a...g..B.n#\n"
  //                        "########.########\n"
  //                        "#l.F..d...h..C.m#\n"
  //                        "#################");
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
  auto star1 = backtrack(order, found, 0);
  cout << numNodesExplored << " nodes explored.\n";

  cout << "Day 18 star 1 = " << star1 << "\n";
  cout << "Day 18 star 2 = " << star2 << "\n";
  showDistance("");
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