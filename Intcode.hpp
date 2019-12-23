//
// Created by Chris Hartman on 12/1/19.
//

#ifndef ADVENTOFCODE2019_INTCODE_HPP
#define ADVENTOFCODE2019_INTCODE_HPP

#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Intcode {
private:
  void readProgram(std::istream &in);
  long long &par(long long n);

public:
  enum State { INPUT, HALT, CONT };
  explicit Intcode(std::istream &in);
  Intcode::State run(std::vector<long long> const &input = {});
  Intcode::State step();
  long long run(int noun, int verb);
  void reset();
  void enqueueInput(const std::vector<long long> &input);
  void compile();
  void freeplay() { _p[0] = 2; }
  std::vector<long long> getOutput();
  long long getOutputSize() { return _output.size(); }
  long long getInputSize() { return _input.size(); }
  long long probeOutput() { return (_output.empty() ? -1 : _output.front()); }

private:
  std::map<long long, long long> _originalProgram;
  std::map<long long, long long> _p;
  std::queue<long long> _input;
  long long _ip = 0;
  long long _rp = 0;
  std::vector<long long> _output;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
