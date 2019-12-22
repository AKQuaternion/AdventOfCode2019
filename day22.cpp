
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

void dealIncrement(vector<int> &v, int increment) {
  auto w = v;
  for (int i = 0; i < v.size(); ++i)
    w[(i * increment) % v.size()] = v[i];
  v.swap(w);
}

void day22() {
  auto star2 = 0;
  ifstream ifile("../day22.txt");
  //  istringstream ifile("deal with increment 7\n"
  //                      "deal into new stack\n"
  //                      "deal into new stack\n");
  //                         "Result: 0 3 6 9 2 5 8 1 4 7");

  //  istringstream ifile("deal into new stack\n"
  //                      "cut -2\n"
  //                      "deal with increment 7\n"
  //                      "cut 8\n"
  //                      "cut -4\n"
  //                      "deal with increment 7\n"
  //                      "cut 3\n"
  //                      "deal with increment 9\n"
  //                      "deal with increment 3\n"
  //                      "cut -1\n");
  //                      "Result: 9 2 5 8 1 4 7 0 3 6");
  vector<int> v(10007);
  iota(v.begin(), v.end(), 0);

  string line;
  while (getline(ifile, line)) {
    string _s;
    istringstream iline(line);
    iline >> _s;
    if (_s == "cut") {
      int i;
      iline >> i;
      std::rotate(v.begin(), v.begin() + (i + v.size()) % v.size(), v.end());
    } else {
      assert(_s == "deal");
      iline >> _s;
      if (_s == "with") {
        int i;
        iline >> _s >> i;
        assert(_s == "increment");
        dealIncrement(v, i);
      } else {
        assert(_s == "into");
        std::reverse(v.begin(), v.end());
      }
    }
  }

  //  for(auto i:v)
  //    cout << i << " ";
  //  cout << endl;

  auto star1 = std::find(v.begin(), v.end(), 2019) - v.begin();
  cout << "Day 22 star 1 = " << star1 << "\n";
  cout << "Day 22 star 2 = " << star2 << "\n";
} // 2808 is too low

// cut -135
// deal with increment 38
// deal into new stack
// deal with increment 29
// cut 120
// deal with increment 30
// deal into new stack