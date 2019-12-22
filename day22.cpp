
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

using Number = long long;

Number numCards = 119'315'717'514'047;
Number numTimes = 101'741'582'076'661;

void xgcd(Number *result, Number a, Number b) {
  Number aa[2] = {1, 0}, bb[2] = {0, 1}, q;
  while (true) {
    q = a / b;
    a = a % b;
    aa[0] = aa[0] - q * aa[1];
    bb[0] = bb[0] - q * bb[1];
    if (a == 0) {
      result[0] = b;
      result[1] = aa[1];
      result[2] = bb[1];
      return;
    };
    q = b / a;
    b = b % a;
    aa[1] = aa[1] - q * aa[0];
    bb[1] = bb[1] - q * bb[0];
    if (b == 0) {
      result[0] = a;
      result[1] = aa[0];
      result[2] = bb[0];
      return;
    };
  };
}

Number inverse(Number x) {
  Number e[3];
  xgcd(e, x, numCards);
  return e[1];
}

void day22() {
  numCards = 10007;
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

  string aline;
  vector<string> shuffle;
  while (getline(ifile, aline)) {
    shuffle.push_back(aline);
  }

  Number mul = 1, add = 0;
  for (const auto &line : shuffle) {
    string _s;
    istringstream iline(line);
    iline >> _s;
    if (_s == "cut") {
      int i;
      iline >> i;
      add += (numCards - i);
      add %= numCards;
    } else {
      assert(_s == "deal");
      iline >> _s;
      if (_s == "with") {
        int i;
        iline >> _s >> i;
        assert(_s == "increment");
        mul = mul * i % numCards;
        add = add * i % numCards;
      } else {
        assert(_s == "into");
        mul = numCards - mul;
        add = numCards - 1 - add;
      }
    }
  }

  auto star1 = (2019 * mul + add) % numCards;
  cout << "Day 22 star 1 = " << star1 << "\n";

  mul = 1;
  add = 0;
  for (auto t = shuffle.rbegin(); t != shuffle.rend(); ++t) {
    const auto &line = *t;
    string _s;
    istringstream iline(line);
    iline >> _s;
    if (_s == "cut") {
      int i;
      iline >> i;
      add += i;
      add %= numCards;
    } else {
      assert(_s == "deal");
      iline >> _s;
      if (_s == "with") {
        int i;
        iline >> _s >> i;
        assert(_s == "increment");
        mul = mul * inverse(i) % numCards;
        add = add * inverse(i) % numCards;
      } else {
        assert(_s == "into");
        mul = numCards - mul;
        add = numCards - 1 - add;
      }
    }
  }
  mul = (mul + numCards) % numCards;
  auto star2 = (4086 * mul + add) % numCards;
  cout << "Day 22 star 2 = " << star2 << "\n";
}

// cut -135
// deal with increment 38
// deal into new stack
// deal with increment 29
// cut 120
// deal with increment 30
// deal into new stack