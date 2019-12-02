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
  static std::map<int, int> instructionLength{{1, 4}, {2, 4}, {99, 1}};

  std::vector<int> p{_p};
  p[1] = noun;
  p[2] = verb;
  auto ip = 0;

  while (p[ip] != 99) {
    const auto &p1 = p[ip + 1];
    const auto &p2 = p[ip + 2];
    const auto &p3 = p[ip + 3];
    const auto &p4 = p[ip + 4]; // probably unused
    auto &l1 = p[p1];
    auto &l2 = p[p2];
    auto &l3 = p[p3];
    auto &l4 = p[p4]; // also probably unused
    auto &op = p[ip];
    if (op == 1) {
      l3 = l1 + l2;
    } else if (op == 2) {
      l3 = l1 * l2;
    } else if (op == 99) {
      return p[0];
    } else {
      throw(
          std::runtime_error("Unimplemented Intcode opcode in Intcode::run()"));
    }
    //    switch (op) {
    //    case 1:
    //      l3 = l1 + l2;
    //      break;
    //    case 2:
    //      l3 = l1 * l2;
    //      break;
    //    case 99:
    //      return p[0];
    //    default:
    //      throw(
    //          std::runtime_error("Unimplemented Intcode opcode in
    //          Intcode::run()"));
    //    }
    ip += instructionLength[p[ip]];
  }
  return p[0];
}

void Intcode::readProgram(std::istream &in) {
  _p.clear();
  while (in) {
    char c;
    int i;
    in >> i >> c;
    _p.push_back(i);
  }
}
