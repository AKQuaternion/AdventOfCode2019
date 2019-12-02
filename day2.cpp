//
// Created by Chris Hartman on 12/1/19.
//
#include <fstream>
#include <iostream>
#include <vector>

int runIntcodeProgram(std::vector<int> program, int noun, int verb) {
  program[1] = noun;
  program[2] = verb;
  auto ip = 0;
  while (program[ip] != 99) {
    auto p1 = program[ip + 1];
    auto p2 = program[ip + 2];
    auto p3 = program[ip + 3];
    if (program[ip] == 1)
      program[p3] = program[p1] + program[p2];
    else {
      assert(program[ip] == 2);
      program[p3] = program[p1] * program[p2];
    }
    ip += 4;
  }
  return program[0];
}

void day2() {
  std::ifstream ifile("../input.txt");
  std::vector<int> originalProgram;
  while (ifile) {
    char c;
    int i;
    ifile >> i >> c;
    originalProgram.push_back(i);
  }

  auto star1 = runIntcodeProgram(originalProgram,12,2);

  auto star2 = 0;
  for (int noun = 0; noun < 100; ++noun)
    for (int verb = 0; verb < 100; ++verb)
     if (runIntcodeProgram(originalProgram,noun,verb)== 19690720)
        star2 = noun*100+verb;

  std::cout << "Day 2 star 1 = " << star1 << "\n";
  std::cout << "Day 2 star 2 = " << star2 << "\n";
}
