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
  //  explicit Intcode(const std::string &s);
  explicit Intcode(std::istream &in);
  std::vector<int> run(std::vector<int> const &input = {});
  int run(int noun, int verb);
  void reset();

private:
  std::vector<int> _originalProgram;
  std::vector<int> _p;
  int _ip = 0;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
