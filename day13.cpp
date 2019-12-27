#include "Intcode.hpp"

//#define USECURSES
// If using curses, run in terminal (not IDE)
// , to slow down, . to speed up
#ifdef USECURSES
#include <chrono>
#include <ncurses.h>
#include <sstream>
#include <thread>
#endif
#include <fstream>
#include <iostream>
#include <numeric>

int sgn(double v) { return (v > 0) - (v < 0); }

void display(int x, int y, int tile) {
#ifdef USECURSES
  char symbol[] = " |#=o";
  if (x != -1)
    mvaddch(y + 1, x, symbol[tile]);
  else {
    std::ostringstream out;
    out << tile;
    move(0, 0);
    addstr(out.str().c_str());
  }
#endif
}

void day13() {
#ifdef USECURSES
  initscr();
  nodelay(stdscr, TRUE);
  noecho();
  curs_set(0);
  clear();
  double frameTime = 10; // milliseconds
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
  program.freeplay(); // TODO !!! rename this, it's used elsewhere, should just
                      // be a "set" for Intcode
  int ballPos = 0;
  int paddlePos = 0;

  std::vector<long long> input;

  while (true) {
    auto state = program.run(input);
    out = program.getOutput();
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
#ifdef USECURSES
    auto ch = getch();
    if (ch == ',')
      frameTime *= 1.1;
    if (ch == '.')
      frameTime /= 1.1;
    std::this_thread::sleep_for(std::chrono::milliseconds(long(frameTime)));
    refresh();
#endif
    input = {sgn(ballPos - paddlePos)};
  }

#ifdef USECURSES
  endwin();
#endif

  std::cout << "Day 13 star 1 = " << star1 << "\n";
  std::cout << "Day 13 star 2 = " << star2 << "\n";
}