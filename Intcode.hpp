//
// Created by Chris Hartman on 12/1/19.
//

#ifndef ADVENTOFCODE2019_INTCODE_HPP
#define ADVENTOFCODE2019_INTCODE_HPP

#include <iostream>
#include <string>
#include <vector>

class Intcode {
private:
  void readProgram(std::istream &in);

public:
  explicit Intcode(const std::string &s);
  explicit Intcode(std::vector<int> p);
  explicit Intcode(std::istream &in);
  std::vector<long long> run(std::vector<long long> const &input);
  int run(int noun, int verb);

private:
  std::vector<long long> doRun(std::vector<int> &p,
                               std::vector<long long> const &input);
  std::vector<int> _p;
  int ip = 0;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
