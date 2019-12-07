//
// Created by Chris Hartman on 12/1/19.
//

#ifndef ADVENTOFCODE2019_INTCODE_HPP
#define ADVENTOFCODE2019_INTCODE_HPP

#include <iostream>
#include <queue>
#include <string>
#include <vector>

class Intcode {
private:
  void readProgram(std::istream &in);

public:
  enum State { INPUT, OUTPUT, HALT };
  explicit Intcode(std::istream &in);
  std::pair<State, int> run(std::vector<int> const &input = {});
  int run(int noun, int verb);
  void reset();
  void enqueueInput(const std::vector<int> &input);
  [[nodiscard]] int getLastOutput() const;

private:
  std::vector<int> _originalProgram;
  std::vector<int> _p;
  std::queue<int> _input;
  int _ip = 0;
  int _lastOutput = 0;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
