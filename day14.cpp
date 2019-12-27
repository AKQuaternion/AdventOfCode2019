#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Number = unsigned long;

struct Ingredient {
  Number amountRequired;
  std::string ingredientName;
};

struct Reaction {
  Number makes;
  std::vector<Ingredient> ingredients;
};

void topoHelper(const std::string &name,
                const std::map<std::string, Reaction> &reactions,
                std::set<std::string> &visited,
                std::deque<std::string> &order) {
  if (name == "ORE")
    return;
  visited.insert(name);
  const auto &[howmuch, ingredients] = reactions.at(name);
  for (const auto &[used, ingredientName] : ingredients)
    if (!visited.count(ingredientName))
      topoHelper(ingredientName, reactions, visited, order);
  order.push_front(name);
}

std::deque<std::string>
topoSort(const std::string &name,
         const std::map<std::string, Reaction> &reactions) {
  std::set<std::string> visited;
  std::deque<std::string> order;
  topoHelper(name, reactions, visited, order);
  return order;
}

std::map<std::string, Reaction> readInput(std::istream &ifile) {
  std::map<std::string, Reaction> reactions;
  std::string line;
  while (getline(ifile, line)) {
    Ingredient i{0, ""};
    std::vector<Ingredient> ingredients;
    std::istringstream iline(line);
    bool done = false;
    while (!done) {
      iline >> i.amountRequired >> i.ingredientName;
      if (i.ingredientName.back() == ',')
        i.ingredientName.pop_back();
      else
        done = true;
      ingredients.push_back(i);
    }
    std::string arrow;
    iline >> arrow >> i.amountRequired >> i.ingredientName; // "=>" num reagent
    reactions[i.ingredientName] = {i.amountRequired, ingredients};
  }
  return reactions;
}

void day14() {
  std::ifstream ifile("../day14.txt");
  //    istringstream ifile("9 ORE => 2 A\n"
  //                        "8 ORE => 3 B\n"
  //                        "7 ORE => 5 C\n"
  //                        "3 A, 4 B => 1 AB\n"
  //                        "5 B, 7 C => 1 BC\n"
  //                        "4 C, 1 A => 1 CA\n"
  //                        "2 AB, 3 BC, 4 CA => 1 FUEL");
  //  istringstream ifile(
  //      "157 ORE => 5 NZVS\n"
  //      "165 ORE => 6 DCFZ\n"
  //      "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
  //      "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
  //      "179 ORE => 7 PSHF\n"
  //      "177 ORE => 5 HKGWZ\n"
  //      "7 DCFZ, 7 PSHF => 2 XJWVT\n"
  //      "165 ORE => 2 GPVTF\n"
  //      "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT");

  auto reactions = readInput(ifile);

  auto intDivRoundUp = [](Number num, Number den) {
    return (num + den - 1) / den;
  };

  auto chemicalsInOrder = topoSort("FUEL", reactions);

  auto oreNeededForFuel = [&](Number fuel) {
    std::map<std::string, Number> amountNeeded{{"FUEL", fuel}};
    for (const auto &chemical : chemicalsInOrder) {
      const auto &[amountReactionMakes, ingredients] = reactions[chemical];
      for (const auto &[reactionRequires, name] : ingredients) {
        auto timesToRunReaction =
            intDivRoundUp(amountNeeded[chemical], amountReactionMakes);
        amountNeeded[name] += reactionRequires * timesToRunReaction;
      }
    }
    return amountNeeded["ORE"];
  };

  std::cout << "Day 14 star 1 = " << oreNeededForFuel(1) << "\n";
  auto lower = 0ull;
  auto upper = 1000000000000ull;
  while (lower != upper) {
    auto mid = lower + (upper - lower) / 2;

    if (oreNeededForFuel(mid) <= 1000000000000ull)
      lower = mid + 1;
    else
      upper = mid;
  }

  std::cout << "Day 14 star 2 = " << lower - 1 << "\n";
}
