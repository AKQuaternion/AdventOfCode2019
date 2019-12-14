//
// Created by Chris Hartman on 12/12/19.
//
#include "Intcode.hpp"

//#define USECURSES

#ifdef USECURSES
#include <curses.h>
#endif
#include <fstream>
#include <iostream>
#include <numeric>

int sgn(double v) { return (v > 0) - (v < 0); }

void display(int x, int y, int tile) {
#ifdef USECURSES
  char symbol[] = " |#=o";

  for (auto row = 0; row < 25; ++row) {
    for (auto col = 0; col < 80; ++col) {
      move(row, col);
      addch((symbol[grid[{col, row}]]));
    }
  }
  move(0, 0);
  std::ostringstream out;
  out << score;
  addstr(out.str().c_str());
  refresh();
#endif
}

void day13() {
#ifdef USECURSES
  initscr();
  clear();
#endif
  auto star1 = 0;
  auto star2 = 0;
  std::ifstream ifile("../day13.txt");
  Intcode program(ifile);
  program.run();
  auto out = program.getOutput();
  for (int i = 0; i < out.size(); i += 3)
    if (out[i + 2] == 2)
      ++star1;

  program.reset();
  program.freeplay();
  int ballPos = 0;
  int paddlePos = 0;

  std::vector<long long> input;

  while (true) {
    auto state = program.run(input);
    auto out = program.getOutput();
    for (int i = 0; i < out.size();) {
      auto x = out[i++];
      auto y = out[i++];
      auto tile = out[i++];
      if (x == -1 && y == 0)
        star2 = tile;
      else if (tile == 4)
        ballPos = x;
      else if (tile == 3)
        paddlePos = x;
      display(x, y, tile);
    }
    if (state == Intcode::HALT)
      break;
    input = {sgn(ballPos - paddlePos)};
  }

  std::cout << "Day 13 star 1 = " << star1 << "\n";
  std::cout << "Day 13 star 2 = " << star2 << "\n";
}