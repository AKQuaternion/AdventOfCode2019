//
// Created by Chris Hartman on 12/1/19.
//

#include "Intcode.hpp"
#include <exception>
#include <map>
#include <sstream>
#include <utility>

Intcode::Intcode(std::vector<int> p) : _p(std::move(p)) {}

Intcode::Intcode(const std::string &s) {
  std::istringstream in{s};
  readProgram(in);
}

Intcode::Intcode(std::istream &in) { readProgram(in); }

int Intcode::run(int noun, int verb) {
  std::vector<int> p{_p};
  p[1] = noun;
  p[2] = verb;
  doRun(p, {});
  return p[0];
}

std::vector<int> Intcode::run(std::vector<int> const &input) {
  std::vector<int> p{_p};
  return doRun(p, input);
}

std::vector<int> Intcode::doRun(std::vector<int> &p,
                                std::vector<int> const &input) {
  auto inIndex = 0;
  auto ip = 0;
  std::vector<int> output;
  while (ip < p.size()) {
    auto instruction = p[ip];
    auto op = instruction % 100;
    if (op == 99)
      break;
    instruction /= 100;
    int dummy = 0;
    auto &p1 = (ip + 1) < p.size() ? p[ip + 1] : dummy;
    auto &p2 = (ip + 2) < p.size() ? p[ip + 2] : dummy;
    auto &p3 = (ip + 3) < p.size() ? p[ip + 3] : dummy;

    auto &l1 = instruction % 10 == 0 ? p[p1] : p1;
    instruction /= 10;
    auto &l2 = instruction % 10 == 0 ? p[p2] : p2;
    instruction /= 10;
    auto &l3 = instruction % 10 == 0 ? p[p3] : p3;
    if (op == 1) { // add
      l3 = l1 + l2;
      ip += 4;
    } else if (op == 2) { // multiply
      l3 = l1 * l2;
      ip += 4;
    } else if (op == 3) { // input
      l1 = input[inIndex++];
      ip += 2;
    } else if (op == 4) { // output
      ip += 2;
      output.push_back(l1);
    } else if (op == 5) { // jump-if-true
      if (l1 != 0)
        ip = l2;
      else
        ip += 3;
    } else if (op == 6) { // jump-if-false
      if (l1 == 0)
        ip = l2;
      else
        ip += 3;
    } else if (op == 7) { // less than
      if (l1 < l2)
        l3 = 1;
      else
        l3 = 0;
      ip += 4;
    } else if (op == 8) { // equals
      if (l1 == l2)
        l3 = 1;
      else
        l3 = 0;
      ip += 4;
    } else {
      throw(
          std::runtime_error("Unimplemented Intcode opcode in Intcode::run()"));
    }
  }
  return output;
}

void Intcode::readProgram(std::istream &in) {
  _p.clear();
  while (in) {
    char c;
    int i;
    in >> i >> c;
    _p.push_back(i);
  }
} // not 1679302
