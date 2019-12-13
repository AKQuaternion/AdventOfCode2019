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
  enum State { INPUT, HALT };
  explicit Intcode(std::istream &in);
  Intcode::State run(std::vector<long long> const &input = {});
  long long run(int noun, int verb);
  void reset();
  void enqueueInput(const std::vector<long long> &input);
  void compile();
  void freeplay() { _p[0] = 2; }
  std::vector<long long> getOutput();
  void setInputGetter(std::function<std::vector<long long>()>);

private:
  std::map<long long, long long> _originalProgram;
  std::map<long long, long long> _p;
  std::queue<long long> _input;
  long long _ip = 0;
  long long _rp = 0;
  std::vector<long long> _output;
  std::function<std::vector<long long>()> _getInput;
};

#endif // ADVENTOFCODE2019_INTCODE_HPP
