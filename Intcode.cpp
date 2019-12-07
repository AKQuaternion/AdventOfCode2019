//
// Created by Chris Hartman on 12/1/19.
//

#include "Intcode.hpp"
#include <exception>
#include <map>
#include <sstream>
#include <utility>

Intcode::Intcode(std::istream &in) { readProgram(in); }

int Intcode::run(int noun, int verb) {
  reset();
  _p[1] = noun;
  _p[2] = verb;
  run();
  return _p[0];
}

std::pair<Intcode::State, int> Intcode::run(std::vector<int> const &input) {
  enqueueInput(input);
  while (_ip < _p.size()) {
    auto instruction = _p[_ip];
    auto op = instruction % 100;
    if (op == 99)
      return {HALT, _lastOutput};
    instruction /= 100;
    int dummy = 0;
    auto &p1 = (_ip + 1) < _p.size() ? _p[_ip + 1] : dummy;
    auto &p2 = (_ip + 2) < _p.size() ? _p[_ip + 2] : dummy;
    auto &p3 = (_ip + 3) < _p.size() ? _p[_ip + 3] : dummy;

    auto &l1 = instruction % 10 == 0 ? _p[p1] : p1;
    instruction /= 10;
    auto &l2 = instruction % 10 == 0 ? _p[p2] : p2;
    instruction /= 10;
    auto &l3 = instruction % 10 == 0 ? _p[p3] : p3;
    if (op == 1) { // add
      l3 = l1 + l2;
      _ip += 4;
    } else if (op == 2) { // multiply
      l3 = l1 * l2;
      _ip += 4;
    } else if (op == 3) { // input
      if (input.empty())
        return {INPUT, _lastOutput};
      l1 = _input.front();
      _input.pop();
      _ip += 2;
    } else if (op == 4) { // output
      _ip += 2;
      _lastOutput = l1;
      return {OUTPUT, l1};
    } else if (op == 5) { // jump-if-true
      if (l1 != 0)
        _ip = l2;
      else
        _ip += 3;
    } else if (op == 6) { // jump-if-false
      if (l1 == 0)
        _ip = l2;
      else
        _ip += 3;
    } else if (op == 7) { // less than
      if (l1 < l2)
        l3 = 1;
      else
        l3 = 0;
      _ip += 4;
    } else if (op == 8) { // equals
      if (l1 == l2)
        l3 = 1;
      else
        l3 = 0;
      _ip += 4;
    } else {
      throw(
          std::runtime_error("Unimplemented Intcode opcode in Intcode::run()"));
    }
  }
  return {HALT, _lastOutput};
}

void Intcode::readProgram(std::istream &in) {
  _originalProgram.clear();
  while (in) {
    char c;
    int i;
    in >> i >> c;
    _originalProgram.push_back(i);
  }
  reset();
}

void Intcode::reset() {
  _ip = 0;
  _p = _originalProgram;
  _input = {};
}

void Intcode::enqueueInput(const std::vector<int> &input) {
  for (auto i : input)
    _input.push(i);
}

int Intcode::getLastOutput() const { return _lastOutput; }
