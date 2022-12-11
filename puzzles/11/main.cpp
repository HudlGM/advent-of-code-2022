#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

const std::string FILENAME_EXAMPLE{"puzzles/11/example.txt"};
const std::string FILENAME_INPUT{"puzzles/11/input.txt"};

const std::string PREFIX_MONKEY{"Monkey"};
const std::string PREFIX_STARTING_ITEMS{"  Starting items:"};
const std::string PREFIX_OPERATION_NEW_EQUALS_OLD_TIMES{"  Operation: new = old * "};
const std::string PREFIX_OPERATION_NEW_EQUALS_OLD_PLUS{"  Operation: new = old + "};
const std::string PREFIX_TEST_DIVISIBLE_BY{"  Test: divisible by "};
const std::string PREFIX_IF_TRUE_THROW_TO_MONKEY{"    If true: throw to monkey "};
const std::string PREFIX_IF_FALSE_THROW_TO_MONKEY{"    If false: throw to monkey "};
const std::string OLD{"old"};
const char COMMA{','};

struct Monkey
{
  Monkey()
    : itemInspectionCount{0}
    , inspectExponent{1}
    , inspectMultiplier{1}
    , inspectAdd{0}
    , testDivisor{1}
    , passOnTrue{0}
    , passOnFalse{0}
  {
  }

  ~Monkey()
  {
  }

  void inspectAndTest(std::vector<Monkey> &monkeys, const long &worryDivisor, const long &productOfAllTestDivisors)
  {
    itemInspectionCount++;

    // inspect
    const long oldWorryLevel = items.at(0);
    items.erase(items.begin());

    long newWorryLevel = oldWorryLevel;
    if (inspectExponent != 1) {
      newWorryLevel = std::pow(oldWorryLevel, inspectExponent);
    } else if (inspectMultiplier != 1) {
      newWorryLevel = oldWorryLevel * inspectMultiplier;
    } else if (inspectAdd != 0) {
      newWorryLevel = oldWorryLevel + inspectAdd;
    }

    newWorryLevel /= worryDivisor;

    // actually do test and pass to next monkey
    if ((newWorryLevel % testDivisor) == 0) {
      monkeys.at(static_cast<size_t>(passOnTrue)).items.push_back(newWorryLevel % productOfAllTestDivisors);
    } else {
      monkeys.at(static_cast<size_t>(passOnFalse)).items.push_back(newWorryLevel % productOfAllTestDivisors);
    }
  }

  std::vector<long> items;
  long itemInspectionCount;
  long inspectExponent;
  long inspectMultiplier;
  long inspectAdd;
  long testDivisor;
  long passOnTrue;
  long passOnFalse;
};

long computeLevelOfMonkeyBusiness(const std::string &filename, const long &totalRounds, const long &worryDivisor);

long computeLevelOfMonkeyBusiness(const std::string &filename, const long &totalRounds, const long &worryDivisor)
{
  std::vector<Monkey> monkeys;

  std::ifstream in(filename);
  std::string l; // line
  std::getline(in, l);
  while (in) {
    if (l.find(PREFIX_MONKEY) != std::string::npos) {
      monkeys.push_back(Monkey());
    } else if (l.find(PREFIX_STARTING_ITEMS) != std::string::npos) {
      // add items
      std::string lPartial = l.substr(PREFIX_STARTING_ITEMS.size());
      std::string::size_type pos = lPartial.find(COMMA);
      while (pos != std::string::npos) {
        // consume next item
        monkeys.back().items.push_back(std::stol(lPartial.substr(0, pos)));
        lPartial = lPartial.substr(pos+1);
        pos = lPartial.find(COMMA);
      }

      // consume last item
      monkeys.back().items.push_back(std::stol(lPartial));
    } else if (l.find(PREFIX_OPERATION_NEW_EQUALS_OLD_TIMES) != std::string::npos) {
      if (l.substr(PREFIX_OPERATION_NEW_EQUALS_OLD_TIMES.size()) == OLD) {
        // set inspect exponent
        monkeys.back().inspectExponent = 2;
      } else {
        // set inspect multiplier
        monkeys.back().inspectMultiplier = std::stol(l.substr(PREFIX_OPERATION_NEW_EQUALS_OLD_TIMES.size()));
      }
    } else if (l.find(PREFIX_OPERATION_NEW_EQUALS_OLD_PLUS) != std::string::npos) {
      // set inspect add
      monkeys.back().inspectAdd = std::stol(l.substr(PREFIX_OPERATION_NEW_EQUALS_OLD_PLUS.size()));
    } else if (l.find(PREFIX_TEST_DIVISIBLE_BY) != std::string::npos) {
      // set test divisor
      monkeys.back().testDivisor = std::stol(l.substr(PREFIX_TEST_DIVISIBLE_BY.size()));
    } else if (l.find(PREFIX_IF_TRUE_THROW_TO_MONKEY) != std::string::npos) {
      // set which monkey to pass to when test is true
      monkeys.back().passOnTrue = std::stol(l.substr(PREFIX_IF_TRUE_THROW_TO_MONKEY.size()));
    } else if (l.find(PREFIX_IF_FALSE_THROW_TO_MONKEY) != std::string::npos) {
      // set which monkey to pass to when test is false
      monkeys.back().passOnFalse = std::stol(l.substr(PREFIX_IF_FALSE_THROW_TO_MONKEY.size()));
    }

    std::getline(in, l);
  }

  if (monkeys.size() < 2) {
    std::cerr << "Not enough monkeys" << std::endl;
    return 0;
  }

#ifdef VERBOSE
  for (size_t i=0; i < monkeys.size(); ++i) {
    Monkey &m = monkeys.at(i);
    std::cout << "[debug] monkey " << i << " starting items(";
    for (size_t j=0; j < m.items.size(); ++j) {
      if (j != 0) {
        std::cout << ", ";
      }

      std::cout << m.items.at(j);
    }
    std::cout << ") e(" << m.inspectExponent << ") m(" << m.inspectMultiplier << ") a(" << m.inspectAdd << ") d(" << m.testDivisor << ") t(" << m.passOnTrue << ") f(" << m.passOnFalse << ")" << std::endl;
  }
#endif

  long productOfAllTestDivisors = 1;
  for (Monkey &m : monkeys) {
    productOfAllTestDivisors *= m.testDivisor;
  }

  for (long round = 0; round < totalRounds; ++round) {
    for (Monkey &m : monkeys) {
      while (m.items.size() > 0) {
        m.inspectAndTest(monkeys, worryDivisor, productOfAllTestDivisors);
      }
    }

#ifdef VERBOSE
    if (
      ((round+1) == 1) ||
      ((round+1) == 20) ||
      ((round+1) == 1000) ||
      ((round+1) == 2000) ||
      ((round+1) == 3000) ||
      ((round+1) == 4000) ||
      ((round+1) == 5000) ||
      ((round+1) == 6000) ||
      ((round+1) == 7000) ||
      ((round+1) == 8000) ||
      ((round+1) == 9000) ||
      ((round+1) == 10000)
    ) {
      std::cout << "== After round " << (round+1) << " ==" << std::endl;
      for (size_t i=0; i < monkeys.size(); ++i) {
        Monkey &m = monkeys.at(i);
        std::cout << "Monkey " << i << " inspected items " << m.itemInspectionCount << " times." << std::endl;
      }
      std::cout << std::endl;
    }
#endif
  }

  std::vector<long> itemInspectionCounts;
  for (Monkey &m : monkeys) {
    itemInspectionCounts.push_back(m.itemInspectionCount);
  }
  std::sort(itemInspectionCounts.begin(), itemInspectionCounts.end(), std::greater());

#ifdef VERBOSE
  std::cout << "[debug] item inspection counts ";
  for (long &i : itemInspectionCounts) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
#endif

  const long levelOfMonkeyBusiness = itemInspectionCounts.at(0) * itemInspectionCounts.at(1);

#ifdef VERBOSE
  std::cout << "[debug] level of monkey business " << levelOfMonkeyBusiness << std::endl;
#endif

  return levelOfMonkeyBusiness;
}

int main()
{
  assert(computeLevelOfMonkeyBusiness(FILENAME_EXAMPLE, 20, 3) == 10605);
  assert(computeLevelOfMonkeyBusiness(FILENAME_EXAMPLE, 10000, 1) == 2713310158);

  std::cout << "level of monkey business (part 1):" << std::endl << computeLevelOfMonkeyBusiness(FILENAME_INPUT, 20, 3) << std::endl;
  std::cout << "level of monkey business (part 2):" << std::endl << computeLevelOfMonkeyBusiness(FILENAME_INPUT, 10000, 1) << std::endl;

  return 0;
}