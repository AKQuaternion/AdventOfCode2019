#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

void day4() {
  auto star1 = 0;
  auto star2 = 0;

  for (int i = 353096; i <= 843212; ++i) {
    std::ostringstream digitsStream;
    digitsStream << i;
    const std::string &digits = digitsStream.str();
    if (!std::is_sorted(digits.begin(), digits.end()))
      continue;
    auto numTimes = [&](auto d) {
      return std::count(digits.begin(), digits.end(), d);
    };
    if (std::any_of(digits.begin(), digits.end(),
                    [&](auto d) { return numTimes(d) >= 2; }))
      ++star1;
    if (std::any_of(digits.begin(), digits.end(),
                    [&](auto d) { return numTimes(d) == 2; }))
      ++star2;
  }

  std::cout << "Day 4 star 1 = " << star1 << "\n";
  std::cout << "Day 4 star 2 = " << star2 << "\n";
}