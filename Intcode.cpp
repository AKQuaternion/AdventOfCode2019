//
// Created by Chris Hartman on 12/1/19.
//

#include "Intcode.hpp"
#include <exception>
#include <map>
#include <sstream>
#include <utility>

Intcode::Intcode(std::istream &in) { readProgram(in); }

long long Intcode::run(int noun, int verb) {
  reset();
  _p[1] = noun;
  _p[2] = verb;
  run();
  return _p[0];
}

long long &Intcode::par(long long n) {
  const static int modeDiv[] = {1, 100, 1000, 10000};
  auto mode = (_p[_ip] / modeDiv[n]) % 10;
  switch (mode) {
  case 0:
    return _p[_p[_ip + n]]; // position
  case 1:
    return _p[_ip + n]; // immediate
  case 2:
    return _p[_rp + _p[_ip + n]]; // relative
  default:
    throw std::runtime_error("Unimplemented mode in Intcode::run()");
  }
}

Intcode::State Intcode::run(std::vector<long long> const &input) {
  enqueueInput(input);
  while (_ip < _p.size())
    switch (_p[_ip] % 100) {
    case 1: // add
      par(3) = par(1) + par(2);
      _ip += 4;
      break;
    case 2: // multiply
      par(3) = par(1) * par(2);
      _ip += 4;
      break;
    case 3: // input
      if (_input.empty())
        return INPUT;
      par(1) = _input.front();
      _input.pop();
      _ip += 2;
      break;
    case 4: // output
      _output.push_back(par(1));
      _ip += 2;
      break;
    case 5: // jump-if-true
      _ip = (par(1) != 0) ? par(2) : _ip + 3;
      break;
    case 6: // jump-if-false
      _ip = (par(1) == 0) ? par(2) : _ip + 3;
      break;
    case 7: // less than
      par(3) = (par(1) < par(2)) ? 1 : 0;
      _ip += 4;
      break;
    case 8: // equals
      par(3) = (par(1) == par(2)) ? 1 : 0;
      _ip += 4;
      break;
    case 9: // adjust rp
      _rp += par(1);
      _ip += 2;
      break;
    case 99:
      return HALT;
    default:
      throw(
          std::runtime_error("Unimplemented Intcode opcode in Intcode::run()"));
    }
  return HALT;
}

void Intcode::readProgram(std::istream &in) {
  _originalProgram.clear();
  int loc = 0;
  while (in) {
    char c;
    long long i;
    in >> i >> c;
    _originalProgram[loc++] = i;
  }
  reset();
}

void Intcode::reset() {
  _ip = 0;
  _rp = 0;
  _p = _originalProgram;
  _input = {};
  _output = {};
}

void Intcode::enqueueInput(const std::vector<long long> &input) {
  for (auto i : input)
    _input.push(i);
}

#include <iostream>
void Intcode::compile() {
  using std::cout;
  using std::to_string;
  int ip = 0;
  auto plabel = [&](const std::string &n) {
    return (n.size() < 3 ? "0" : "") + ((n.size() < 2 ? "0" : "") + n);
  };
  auto par = [&](int n) -> std::string {
    const static int modeDiv[] = {1, 100, 1000, 10000};
    auto mode = (_p[ip] / modeDiv[n]) % 10;
    switch (mode) {
    case 0:
      return "p[" + to_string(_p[ip + n]) + "]"; // position
    case 1:
      return to_string(_p[ip + n]); // immediate
    case 2:
      return "p[rp" +
             (_p[ip + n] >= 0 ? "+" + to_string(_p[ip + n])
                              : "-" + to_string(-_p[ip + n])) +
             "]"; // relative
    default:
      throw std::runtime_error("Unimplemented mode in Intcode::run()");
    }
  };
  for (int i = 0; i < _p.size(); ++i)
    cout << "p[" << i << "] = " << _p[i] << ";\n";
  while (ip < _p.size()) {
    cout << "label" << plabel(std::to_string(ip)) << ":\t";
    switch (_p[ip] % 100) {
    case 1: // add
      if (par(3) == par(1))
        cout << par(3) << " += " << par(2) << ";\n";
      else if (par(3) == par(2))
        cout << par(3) << " += " << par(1) << ";\n";
      else
        cout << par(3) << " = " << par(1) << " + " << par(2) << ";\n";
      ip += 4;
      break;
    case 2: // multiply
      if (par(3) == par(1))
        cout << par(3) << " *= " << par(2) << ";\n";
      else if (par(3) == par(2))
        cout << par(3) << " *= " << par(1) << ";\n";
      else
        cout << par(3) << " = " << par(1) << " * " << par(2) << ";\n";
      ip += 4;
      break;
    case 3: // input
      cout << par(1) << " = *in++;\n";
      ip += 2;
      break;
    case 4: // output
      cout << "output(" << par(1) << ");\n";
      ip += 2;
      break;
    case 5: // jump-if-true
      cout << "if(" << par(1) << " != 0) goto label" << plabel(par(2)) << ";\n";
      ip += 3;
      break;
    case 6: // jump-if-false
      cout << "if(" << par(1) << " == 0) goto label" << plabel(par(2)) << ";\n";
      ip += 3;
      break;
    case 7: // less than
      cout << par(3) << " = (" << par(1) << " < " << par(2) << ") ? 1 : 0;\n";
      ip += 4;
      break;
    case 8: // equals
      cout << par(3) << " = (" << par(1) << " == " << par(2) << ") ? 1 : 0;\n";
      ip += 4;
      break;
    case 9: // set rp
      cout << "rp += " << par(1) << ";\n";
      ip += 2;
      break;
    case 99:
      cout << "return; //HALT\n";
      ip += 1;
      break;
    default:
      cout << "// p" << ip << " = " << _p[ip] << ";\n";
      ip += 1;
      //            throw(
      //                std::runtime_error("Unimplemented Intcode opcode in
      //                Intcode::compile()"));
    }
  }
}

std::vector<long long> Intcode::getOutput() {
  std::vector<long long> temp;
  temp.swap(_output);
  return temp;
}
