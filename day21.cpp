//
// Created by Chris Hartman on 12/20/19.
//
//
// Created by Chris Hartman on 12/16/19.
//
#include "Intcode.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void day21() {
  std::ifstream ifile("../day21.txt");
  Intcode i(ifile);

  auto runASCII = [&](std::string_view s) {
    i.reset();
    std::vector<long long> p;
    copy(s.begin(), s.end(), std::back_inserter(p));
    i.run(p);
    auto out = i.getOutput();
    if (out.back() > 255)
      return out.back();
    std::string asciiOutput;
    std::copy(out.begin(), out.end(), std::back_inserter(asciiOutput));
    std::cout << asciiOutput;
    return 0ll;
  };

  std::string program1{"NOT C J\n" // jump if a hole is three away and it's safe
                       "AND D J\n" // @.. #
                       "NOT A T\n" // jump if a hole is one away:
                       "OR T J\n"  // @ ...
                       "WALK\n"};

  std::string program2{"NOT C T\n"
                       "AND D T\n" // jump if a hole is three away
                       "AND H T\n" // and it's safe to jump twice:
                       "OR T J\n"  // @.. #...#.
                       "NOT B T\n"
                       "AND D T\n" // jump if a hole is two away:
                       "OR T J\n"  // @. .#.....
                       "NOT A T\n" // jump if a hole is one away:
                       "OR T J\n"  // @ ........
                       "RUN\n"};

  auto star1 = runASCII(program1);
  auto star2 = runASCII(program2);

  std::cout << "Day 21 star 1 = " << star1 << "\n";
  std::cout << "Day 21 star 2 = " << star2 << "\n";
}