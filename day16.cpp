#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int digitsToNum(int numDigits, const std::vector<int> &digits) {
  int num = 0;
  for (int i = 0; i < numDigits; ++i)
    num = 10 * num + digits[i];
  return num;
}

void day16() {
  auto star1 = 0;
  auto star2 = 0;
  std::ifstream ifile("../day16.txt");
  std::string line;
  ifile >> line;
  std::vector<int> original;
  for (auto c : line)
    original.push_back(c - '0');

  auto offset = digitsToNum(7, original);

  auto v = original;

  int sequence[] = {0, 1, 0, -1};
  for (int t = 1; t <= 100; ++t) {
    std::vector<int> n;
    for (int i = 0; i < v.size(); ++i) {
      int sum = 0;
      for (int j = 0; j < v.size(); ++j) {
        sum += (sequence[((j + 1) % (4 * (i + 1))) / (i + 1)] * v[j]);
      }
      n.push_back(abs(sum) % 10);
    }
    v = n;
  }

  std::cout << "Day 16 star 1 = " << digitsToNum(8, v) << "\n";

  v.resize(10000 * original.size() - offset);
  for (int s = 0; s < v.size(); ++s)
    v[s] = original[(s + offset) % original.size()];

  for (int i = 0; i < 100; ++i)
    std::partial_sum(v.crbegin(), v.crend(), v.rbegin(),
                     [](int sum, int digit) { return (sum + digit) % 10; });

  std::cout << "Day 16 star 2 = " << digitsToNum(8, v) << "\n";
}
