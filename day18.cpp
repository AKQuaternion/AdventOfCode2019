#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using std::pair;
using std::string;
using std::vector;

class Day18 {
  using Vertex = pair<short, short>;

  int pack(Vertex v) { return v.first << 16 | v.second; }

  // BFS from current robot positions to find possible next keys to pick up
  auto findChoices(const string &order, const vector<Vertex> &robots) {
    vector<std::tuple<int, char, int>>
        result; // which robot, which key, distance
    for (int robot = 0; robot < robots.size(); ++robot) {
      std::unordered_set<int> visited;
      std::queue<pair<Vertex, int>> q; // node, distance
      q.push({robots[robot], 0});
      while (!q.empty()) {
        auto [node, dist] = q.front();
        q.pop();
        if (visited.count(pack(node)))
          continue;                 // don't visit a node we've already seen
        visited.insert(pack(node)); // mark this node as visited
        auto ch = grid[node.first][node.second];
        if (islower(ch) && order.find(ch) == string::npos) { // found a new key
          result.emplace_back(robot, ch, dist); // This is a choice for next key
          continue;                             // don't travel past a new key
        }
        if (isupper(ch) && order.find(tolower(ch)) == string::npos)
          continue; // don't travel past a door we don't have the key for
        for (auto nbr : edges[pack(node)])
          q.push({nbr, dist + 1}); // search neighbor nodes
      }
    }
    return result;
  }

  int fewestStepsToGetRestOfKeys(string &keysCollected,
                                 vector<Vertex> &robotPositions) {
    if (keysCollected.size() == numKeys)
      return 0;
    ++numNodesExplored;

    // the number of steps to finish depends only on the keys collected so far
    // (not their order!) and the current robot positions. So we memoize that,
    // saving it the first time we compute it, and reusing it.
    auto memoString = keysCollected;
    std::sort(memoString.begin(),
              memoString.end()); // removing order dependence
    for (auto r : robotPositions)
      memoString += grid[r.first][r.second]; // appending robot last key found

    if (memos.count(memoString))   // if we've calculated this before
      return memos.at(memoString); // just use the calculated value

    auto possibleNextChoices = findChoices(keysCollected, robotPositions);
    //
    //  if (numNodesExplored < 100 || keysCollected.size() <= 5) {
    //    std::cout << keysCollected << "    "; // << endl;
    //    for (auto [r, n, dist] : possibleNextChoices)
    //      std::cout << "r" << r << "@" << n << ":" << dist << ",  ";
    //    std::cout << std::endl;
    //  }

    int bestNumSteps = 9999;

    for (auto [robotIndex, key, distance] : possibleNextChoices) {
      keysCollected.push_back(key);
      auto oldRobotPos = robotPositions[robotIndex];
      robotPositions[robotIndex] = keyPosition[key - 'a'];
      auto stepsToFinish =
          distance + fewestStepsToGetRestOfKeys(keysCollected, robotPositions);
      bestNumSteps = std::min(bestNumSteps, stepsToFinish);
      keysCollected.pop_back();
      robotPositions[robotIndex] = oldRobotPos;
    }
    return memos[memoString] = bestNumSteps; // memoize and return
  }

public:
  Day18(std::string filename) {
    std::ifstream ifile("../day18.txt");

    string line;
    while (getline(ifile, line))
      grid.push_back(line);
  }

  auto makeEdgesAndGetRobotPositions() {
    vector<Vertex> robotPositions;
    for (int r = 0; r < grid.size(); ++r)
      for (int c = 0; c < grid[r].size(); ++c) {
        auto ch = grid[r][c];
        if (ch == '#')
          continue;
        if (islower(ch)) { // key
          keyPosition[ch - 'a'] = {r, c};
          numKeys = std::max(numKeys, ch - 'a' + 1);
        } else if (ch == '@')
          robotPositions.emplace_back(r, c);
        auto putEdge = [&](int y, int x) {
          if (grid[y][x] != '#')
            edges[pack({r, c})].push_back({y, x});
        };
        putEdge(r - 1, c);
        putEdge(r + 1, c);
        putEdge(r, c + 1);
        putEdge(r, c - 1);
      }
    return robotPositions;
  }

  auto solve() {
    string keysCollected;
    auto robotPositions = makeEdgesAndGetRobotPositions();
    auto [ry, rx] = robotPositions.front();
    auto star1 = fewestStepsToGetRestOfKeys(keysCollected, robotPositions);

    grid[ry - 1].replace(rx - 1, 3, "@#@");
    grid[ry + 0].replace(rx - 1, 3, "###");
    grid[ry + 1].replace(rx - 1, 3, "@#@");
    numNodesExplored = 0ull;
    memos.clear();
    edges.clear();
    robotPositions = makeEdgesAndGetRobotPositions();
    auto star2 = fewestStepsToGetRestOfKeys(keysCollected, robotPositions);

    std::cout << "Day 18 star 1 = " << star1 << "\n";
    std::cout << "Day 18 star 2 = " << star2 << "\n";
  }

  void prepStar2() { grid.clear(); }

private:
  int numKeys;
  unsigned long long numNodesExplored = 0ull;
  vector<string> grid;
  std::unordered_map<string, int> memos;
  std::unordered_map<int, vector<Vertex>> edges;
  std::array<Vertex, 26> keyPosition;
};

void day18() {
  Day18 day18("../day18.txt");

  day18.solve();
}