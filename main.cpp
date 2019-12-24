#include <chrono>
#include <iostream>

void day1();
void day2();
void day3();
void day4();
void day5();
void day6();
void day7();
void day8();
void day9();
void day10();
void day11();
void day12();
void day13();
void day14();
void day15();
void day16();
void day17();
void day18();
void day19();
void day20();
void day21();
void day22();
void day23();
void day24();
void day25();

// day 1 calculate fuel required
// day 2 run Intcode program
// day 3 wires on a grid U D L R
// day 4 digit passwords
// day 5 more Intcode
// day 6 orbits tree traversal
// day 7 chained Intcode
// day 8 pixel image
// day 9 more Intcode
// day 10 shoot at asteroids
// day 11 Intcode paint the spaceship
// day 12 Moons orbiting each other
// day 13 Intcode Breakout game
// day 14 Chemical reactions tree of requirements
// day 15 Intcode repair droid to model search
// day 16 "FFT" of signals
// day 17 Intcode ASCII vacuum robot on scaffolding
// day 18 Robots picking up keys BFS
// day 19 Intcode Map tractor beam and find square containment
// day 20 Explore a maze
// day 21 Intcode jumping droid
// day 22 Shuffle cards with lots of modular arithmetic
// day 23 Intcode 50 networked NICs which can idle, and a NAT
// day 24
// day 25
void day10old();

double timesec() {
  auto nanos = static_cast<std::chrono::nanoseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
  return static_cast<double>(nanos.count()) / 1.e9;
}
int main() {
  double time1 = timesec(); // End time (sec)

  //  day1();
  //  day2();
  //  day3();
  //  day4();
  //  day5();
  //  day6();
  //  day7();
  //  day8();
  //  day9();
  //  day10();
  //  day10old();
  //  day11();
  //  day12();
  //  day13();
  //  day14();
  //  day15();
  //  day16();
  //  day17();
  //  day18();
  //  day19();
  //  day20();
  //  day21();
  //  day22();
  //  day23();
  day24();
  double time2 = timesec();           // End time (sec)
  double elapsedtime = time2 - time1; // Elapsed (sec)
  std::cout << "Time required: " << elapsedtime << " seconds" << std::endl;
}
// Day 1 star 1 = 3372756
// Day 1 star 2 = 5056279
// Day 2 star 1 = 4138687
// Day 2 star 2 = 6635
// Day 3 star 1 = 308
// Day 3 star 2 = 12934
// Day 4 star 1 = 579
// Day 4 star 2 = 358
// Day 5 star 1 = 16348437
// Day 5 star 2 = 6959377
// Day 6 star 1 = 268504
// Day 6 star 2 = 409
// Day 7 star 1 = 273814
// Day 7 star 2 = 34579864
// Day 8 star 1 = 2440
// Day 8 star 2 =
//   * *     * * * *     * *         * *     * *
// *     *         *   *     *         *   *     *
// *     *       *     *               *   *
// * * * *     *       *               *   *
// *     *   *         *     *   *     *   *     *
// *     *   * * * *     * *       * *       * *
// Day 9 star 1 = 4288078517
// Day 9 star 2 = 69256
// Day 10 star 1 = 296
// Day 10 star 2 = 204
// Day 11 star 1 = 2511
// Day 11 star 2 =
// *     *       * *   *     *       * *   *     *     * *     * * *     *     *
// *     *         *   *   *           *   *   *     *     *   *     *   *     *
// * * * *         *   * *             *   * *       *         *     *   * * * *
// *     *         *   *   *           *   *   *     *   * *   * * *     *     *
// *     *   *     *   *   *     *     *   *   *     *     *   *         *     *
// *     *     * *     *     *     * *     *     *     * * *   *         *     *
// Day 12 star 1 = 13500
// Day 12 star 2 = 278013787106916
// Day 13 star 1 = 376
// Day 13 star 2 = 18509
// Day 14 star 1 = 97422
// Day 14 star 2 = 13108426
// Day 15 star 1 = 318
// Day 15 star 2 = 390
// Day 16 star 1 = 44098263
// Day 16 star 2 = 12482168
// Day 17 star 1 = 10064
// Day 17 star 2 = 1197725
// Day 18 star 1 = 4520
// Day 18 star 2 = 1540
// Day 19 star 1 = 197
// Day 19 star 2 = 919022
// Day 20 star 1 = 686
// Day 20 star 2 = 8384
// Day 21 star 1 = 19355436
// Day 21 star 2 = 1142618405
// Day 22 star 1 = 4086
// Day 22 star 2 = 1041334417227
// Day 23 star 1 = 21160
// Day 23 star 2 = 14331