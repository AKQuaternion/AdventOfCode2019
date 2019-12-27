#include "Intcode.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using std::abs;
using std::ceil;
using std::cout;
using std::endl;
using std::forward_as_tuple;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::map;
using std::max;
using std::max_element;
using std::min;
using std::pair;
using std::queue;
using std::set;
using std::sqrt;
using std::string;
using std::swap;
using std::tie;
using std::tuple;
using std::vector;

void day25() {
  auto star1 = 0;
  auto star2 = 0;
  ifstream ifile("../day25.txt");
  Intcode i(ifile);
  i.run();
  for (int t = 0; t < 1e9; ++t) {
    auto out = i.getOutput();
    string hisOut;
    copy(out.begin(), out.end(), back_inserter(hisOut));
    cout << hisOut;
    string myIn;
    getline(std::cin, myIn);
    vector<long long> input;
    copy(myIn.begin(), myIn.end(), back_inserter(input));
    input.push_back(('\n');
    i.run(input);
  }
  //  string line;
  //  while (getline(ifile, line)) {
  //    string _s;
  //    istringstream iline(line);
  //    iline >> _s;
  //  }

  cout << "Day 25 star 1 = " << star1 << "\n";
  cout << "Day 25 star 2 = " << star2 << "\n";
}
