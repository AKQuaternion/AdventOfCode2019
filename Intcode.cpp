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

int &Intcode::par(int n) {
  const static int modeDiv[] = {1, 100, 1000, 10000};
  return ((_p[_ip] / modeDiv[n]) % 10) ? _p[_ip + n] : _p[_p[_ip + n]];
}

std::pair<Intcode::State, int> Intcode::run(std::vector<int> const &input) {
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
      if (input.empty())
        return {INPUT, _lastOutput};
      par(1) = _input.front();
      _input.pop();
      _ip += 2;
      break;
    case 4: // output
      _lastOutput = par(1);
      _ip += 2;
      return {OUTPUT, _lastOutput};
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
    case 99:
      return {HALT, _lastOutput};
    default:
      throw(
          std::runtime_error("Unimplemented Intcode opcode in Intcode::run()"));
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

#include <iostream>
void Intcode::compile() const {
  using std::cout;
  using std::to_string;
  int ip = 0;

  auto par = [&](int n) -> std::string {
    const static int modeDiv[] = {1, 100, 1000, 10000};
    return ((_p[ip] / modeDiv[n]) % 10) ? "p" + to_string(ip + n)
                                        : "p[" + to_string(_p[ip + n]) + "]";
    //    : "p[p" + to_string(ip + n) + "=" + to_string(_p[ip + n]) + "] ";
  };
  for (int i = 0; i < _p.size(); ++i)
    cout << "int p" << i << " = " << _p[i] << ";\n";
  while (ip < _p.size()) {
    cout << "label" << to_string(ip) << ":\n\t";
    switch (_p[ip] % 100) {
    case 1: // add
      cout << par(3) << " = " << par(1) << " + " << par(2) << ";\n";
      ip += 4;
      break;
    case 2: // multiply
      cout << par(3) << " = " << par(1) << " * " << par(2) << ";\n";
      ip += 4;
      break;
    case 3: // input
      cout << par(1) << " = input; //INPUT\n";
      ip += 2;
      break;
    case 4: // output
      cout << "return " << par(1) << "; //OUTPUT\n";
      ip += 2;
      break;
    case 5: // jump-if-true
      cout << "if(" << par(1) << " != 0) goto label" << par(2) << ";\n";
      ip += 3;
      break;
    case 6: // jump-if-false
      cout << "if(" << par(1) << " == 0) goto label" << par(2) << ";\n";
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
    case 99:
      cout << "return; //HALT\n";
      ip += 1;
      break;
    default:
      cout << "// p" << ip << " = " << _p[ip] << ";\n";
      ip += 1;
      //      throw(
      //          std::runtime_error("Unimplemented Intcode opcode in
      //          Intcode::compile()"));
    }
  }
}
