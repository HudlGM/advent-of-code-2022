#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

int main()
{
  std::ifstream in("1.in");

  std::vector<int> elves;

  bool newElf = true;
  std::string s;
  while (in) {
    if (newElf) {
      elves.push_back(0);
      newElf = false;
    }

    std::getline(in, s);

    if (s.empty() || in.fail()) {
#ifdef VERBOSE
      std::cout << "Elf " << elves.size() << " is holding " << elves.back() << " calories" << std::endl;
#endif
      newElf = true;
    } else {
#ifdef VERBOSE
      std::cout << elves.back() << "+" << std::stoi(s);
#endif
      elves.back() += std::stoi(s);
#ifdef VERBOSE
      std::cout << "=" << elves.back() << std::endl;
#endif
    }
  }

  // Note: solution does not care about elves' identity, so don't worry about knowing which elf is which
  std::sort(elves.begin(), elves.end(), std::greater<int>());

  std::cout << std::endl << "Calorie counts for elves with the most calories: " << std::endl;

  int total = 0;
  for (int i=0; (i < 3) && (i < elves.size()); ++i) {
    std::cout << "  " << elves.at(i) << std::endl;
    total += elves.at(i);
  }

  std::cout << std::endl << "Which totals " << total << std::endl;

  return 0;
}