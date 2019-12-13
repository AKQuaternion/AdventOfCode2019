//
// Created by Chris Hartman on 12/1/19.
//

#ifndef ADVENTOFCODE2019_INTCODE_HPP
#define ADVENTOFCODE2019_INTCODE_HPP

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Intcode {
private:
  void readProgram(std::istream &in);

public:
  enum State { INPUT, OUTPUT, HALT };
  explicit Intcode(std::istream &in);
  std::pair<State, long long> run(std::vector<long long> const &input = {});
  long long run(int noun, int verb);
  void reset();
  void enqueueInput(const std::vector<long long> &input);
  void compile();
  void freeplay() { _p[0] = 2; }

private:
  long long &par(long long n);
  std::map<long long, long long> _originalProgram;
  std::map<long long, long long> _p;
  std::queue<long long> _input;
  long long _ip = 0;
  long long _rp = 0;
  long long _lastOutput = 0;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
