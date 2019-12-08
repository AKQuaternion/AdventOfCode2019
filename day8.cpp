#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void day8() {
  auto star1 = 0;
  std::ifstream ifile("../day8.txt");
  std::vector<std::string> pix;
  std::string layer(6 * 25, ' ');
  auto minZeroes = layer.size();
  while (ifile.read(layer.data(), 6 * 25)) {
    pix.push_back(layer);
    if (auto z = std::count(layer.begin(), layer.end(), '0'); z < minZeroes) {
      minZeroes = z;
      star1 = std::count(layer.begin(), layer.end(), '1') *
              std::count(layer.begin(), layer.end(), '2');
    }
  }

  std::string star2 = pix.front();
  for (const auto &theLayer : pix)
    for (auto i = 0ul; i < star2.size(); ++i)
      star2[i] = (star2[i] != '2' ? star2[i] : theLayer[i]);

  std::cout << "Day 8 star 1 = " << star1 << "\nDay 8 star 2 =";

  for (int i = 0; i < star2.size(); ++i)
    std::cout << ((i % 25 == 0) ? '\n' : ' ') << (star2[i] == '0' ? ' ' : '*');
}
