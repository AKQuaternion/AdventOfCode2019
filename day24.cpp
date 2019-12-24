#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

int nbrs(int r, int c, std::vector<std::string> const &lines) {
  int n = 0;
  if (r > 0)
    n += lines[r - 1][c] == '#';
  if (r < 4)
    n += lines[r + 1][c] == '#';
  if (c > 0)
    n += lines[r][c - 1] == '#';
  if (c < 4)
    n += lines[r][c + 1] == '#';
  return n;
}

unsigned long diversity(std::vector<std::string> const &lines) {
  unsigned long v = 1;
  auto total = 0ul;
  for (int r = 0; r < 5; ++r)
    for (int c = 0; c < 5; ++c) {
      if (lines[r][c] == '#')
        total += v;
      v <<= 1u;
    }
  return total;
}

auto star1(std::vector<std::string> lines) {
  std::map<std::vector<std::string>, int> history;

  auto time = 0;
  history[lines] = time;

  auto other = lines;
  while (true) {
    time++;
    for (int r = 0; r < 5; ++r)
      for (int c = 0; c < 5; ++c)
        other[r][c] = ((nbrs(r, c, lines) == 1) ||
                       (lines[r][c] != '#' && nbrs(r, c, lines) == 2))
                          ? '#'
                          : '.';
    lines = other;
    if (history.count(lines)) {
      return (diversity(lines));
    }
    history[lines] = time;
  }
}

int nbrs(int l, int r, int c,
         std::map<std::tuple<int, int, int>, bool> &grids) {
  if (r == 2 && c == 2)
    return 0;

  int n = 0;

  // up
  if (r == 0)
    n += grids[{l - 1, 1, 2}];
  else if (r == 3 && c == 2) {
    for (int i = 0; i < 5; ++i)
      n += grids[{l + 1, 4, i}];
  } else
    n += grids[{l, r - 1, c}];

  // down
  if (r == 4)
    n += grids[{l - 1, 3, 2}];
  else if (r == 1 && c == 2)
    for (int i = 0; i < 5; ++i)
      n += grids[{l + 1, 0, i}];
  else
    n += grids[{l, r + 1, c}];

  // left
  if (c == 0)
    n += grids[{l - 1, 2, 1}];
  else if (c == 3 && r == 2) {
    for (int i = 0; i < 5; ++i)
      n += grids[{l + 1, i, 4}];
  } else
    n += grids[{l, r, c - 1}];

  // right
  if (c == 4)
    n += grids[{l - 1, 2, 3}];
  else if (c == 1 && r == 2)
    for (int i = 0; i < 5; ++i)
      n += grids[{l + 1, i, 0}];
  else
    n += grids[{l, r, c + 1}];

  return n;
}

auto star2(const std::vector<std::string> &lines) {
  std::map<std::tuple<int, int, int>, bool> grids;
  for (int r = 0; r < 5; ++r)
    for (int c = 0; c < 5; ++c) {
      grids[{0, r, c}] = lines[r][c] == '#';
    }

  auto time = 0;

  auto other = grids;

  for (int t = 0; t < 200; ++t) {
    for (int l = -100; l <= 100; ++l)
      for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c)
          other[{l, r, c}] = (nbrs(l, r, c, grids) == 1) ||
                             (!other[{l, r, c}] && nbrs(l, r, c, grids) == 2);
    grids = other;
  }

  auto star2 = 0;
  for (int l = -100; l <= 100; ++l)
    for (int r = 0; r < 5; ++r)
      for (int c = 0; c < 5; ++c)
        star2 += (grids[{l, r, c}]);

  return star2;
}

void day24() {
  std::ifstream ifile("../day24.txt");

  std::string line;

  std::vector<std::string> lines;

  while (getline(ifile, line)) {
    lines.push_back(line);
  }

  std::cout << "Day 24 star 1 = " << star1(lines) << "\n";
  std::cout << "Day 24 star 2 = " << star2(lines) << "\n";
}