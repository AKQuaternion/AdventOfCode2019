#include <fstream>
#include <gmp.h>
#include <gmpxx.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Number = mpz_class;

Number numCards = 119'315'717'514'047;
long numTimes = 101'741'582'076'661;

enum class Technique { NEWSTACK, CUT, DEAL };
std::pair<Technique, int> parseLine(std::string const &line) {
  std::string word;
  std::istringstream iline(line);
  iline >> word;
  if (word == "cut") {
    int i;
    iline >> i;
    return {Technique::CUT, i};
  } else {
    assert(word == "deal");
    iline >> word;
    if (word == "with") {
      int i;
      iline >> word >> i;
      assert(word == "increment");
      return {Technique::DEAL, i};
    } else {
      assert(word == "into");
      return {Technique::NEWSTACK, 0};
    }
  }
}

Number inverse(Number i) {
  Number inverse;
  mpz_invert(inverse.get_mpz_t(), Number(i).get_mpz_t(), numCards.get_mpz_t());
  return inverse;
}

void day22() {
  std::ifstream ifile("../day22.txt");

  std::string aline;
  std::vector<std::string> shuffle;
  while (getline(ifile, aline)) {
    shuffle.push_back(aline);
  }

  // For part 1
  // we note that all the shuffling operations take the card at position p
  // to a new position equal to (mp+a mod numCards), for some m and a:
  // a cut to i just subtracts i from p, p = p + -i
  // a "new deal" (reversing the deck) sends p = (numCards-1)-p
  // and dealing to every i'th position is p = i*p
  // Keeping those cumulative, we begin with m=1 and a=0:
  // CUT: mx + a -> mx + a-i is a -= i
  // NEWSTACK: mx + a -> (numCards-1-(mx + a)) is m = -m, a = numCards-1-a
  // DEAL: mx + a -> i*(mx + a) is m *= i, a *= i

  Number m1 = 1;
  Number a1 = 0;
  long const numCardsPart1 = 10007;
  for (auto line : shuffle) {
    auto [technique, i] = parseLine(line);
    switch (technique) {
    case Technique::CUT:
      a1 = (a1 + (numCardsPart1 - i)) % numCardsPart1;
      break;
    case Technique::NEWSTACK:
      m1 = numCardsPart1 - m1;
      a1 = numCardsPart1 - 1 - a1;
      break;
    case Technique::DEAL:
      m1 = (m1 * i) % numCardsPart1;
      a1 = (a1 * i) % numCardsPart1;
      break;
    }
  }

  std::cout << "Day 22 star 1 = " << (2019 * m1 + a1) % numCardsPart1
            << std::endl;

  // For part 2, we need to find out what card ends in position 2020. If
  // we do the shuffles *backwards*, we can think of this as "where did 2020
  // go?." So we calculate the inverses of the three shuffle operations.
  // Cut is just using i instead of -i
  // Dealing into a new stack is already self inverse
  // Dealing to every nth card is slightly harder, we must find the inverse of
  // n (mod the number of cards.) This can be done without any "hard" math
  // by just cheking if i*(number of cards)+1 is divisible by n, for 0 <i < n,
  // or one can use the extended euclidean algorithm. (This is feasible for
  // the numbers in our shuffle, as they are all smaller than 100.) In the code
  // for inverse(n) above, I use the GNU MP package, which is necessary for the
  // large numbers involved in this problem anyway, which knows how to
  // use the extended euclidean algorithm.
  //
  // Finally, we have an equation,the card at position p came from mul*p + add
  // iterating "p = m*p+a" n times gives us
  // m^n * p + (m^(n-1) + m^(n-2) + ... + m + 1)*a
  // = m^n * p + ((m^n-1)/(m-1))*a
  // or m^n * p + (m^n-1) * inverse(m-1) * a
  //
  // If you don't understand the derivation above (which uses the sum of
  // a geometric series) you can also iterate the equation twice to get e2
  // iterating e2 twice to get e4, etc. and then applying e1, e2, etc. if
  // the corresponding bit of "number of times to iterate" is set. (For example,
  // if #times = 9, you would apply e8 and e1.)

  Number mul = 1;
  Number add = 0;
  for (auto t = shuffle.rbegin(); t != shuffle.rend(); ++t) {
    auto [technique, i] = parseLine(*t);
    switch (technique) {
    case Technique::CUT:
      add = (add + i) % numCards;
      break;
    case Technique::DEAL:
      mul = (mul * inverse(i)) % numCards;
      add = (add * inverse(i)) % numCards;
      break;
    case Technique::NEWSTACK:
      mul = numCards - mul;
      add = numCards - 1 - add;
      break;
    }
  }

  Number mn;
  mpz_powm_ui(mn.get_mpz_t(), mul.get_mpz_t(), numTimes, numCards.get_mpz_t());
  Number left = mn * 2020;
  Number right = (mn - 1) * inverse(mul - 1) * add;
  std::cout << "Day 22 star 2 = " << (left + right) % numCards << std::endl;
}
