#include <fstream>
#include <iostream>
#include <vector>

char findMisplacedItem(const std::string &c1, const std::string &c2)
{
  for (const char &c : c1) {
    if (c2.find(c) != std::string::npos) {
      return c;
    }
  }

  return '\0';
}

char findBadge(const std::vector<std::string> &group)
{
  for (const char &c : group.at(0)) {
    if (
      (group.at(1).find(c) != std::string::npos) &&
      (group.at(2).find(c) != std::string::npos)
    ) {
#ifdef VERBOSE
      std::cout << "badge is " << c << std::endl;
      std::cout << "  " << group.at(0) << std::endl;
      std::cout << "  " << group.at(1) << std::endl;
      std::cout << "  " << group.at(2) << std::endl;
#endif
      return c;
    }
  }

  return '\0';
}

int getPriority(const char &c)
{
  // Lowercase item types a through z have priorities 1 through 26.
  if ((c >= 'a') && (c <= 'z')) {
    return 1 + c - 'a';
  }

  // Uppercase item types A through Z have priorities 27 through 52.
  return 27 + c - 'A';
}

int main()
{
  std::ifstream in("puzzles/3/input.txt");

  std::string s; // rucksack contents
  std::string c1; // compartment 1
  std::string c2; // compartment 2

#ifdef VERBOSE
  int groupCount = 0;
#endif
  std::vector<std::string> group;

  int prioritySumPart1 = 0;
  int prioritySumPart2 = 0;

  std::getline(in, s);

  while (in) {
    // first half first compartment
    c1 = s.substr(0,s.length()/2);

    // second half second compartment
    c2 = s.substr(s.length()/2);

    prioritySumPart1 += getPriority(findMisplacedItem(c1,c2));

    group.push_back(s);

    if (group.size() == 3) {
#ifdef VERBOSE
      std::cout << "Group " << ++groupCount << " ";
#endif
      prioritySumPart2 += getPriority(findBadge(group));
      group.clear();
    }

    std::getline(in, s);
  }

  std::cout << std::endl << "priority sum part 1: " << prioritySumPart1 << std::endl;
  std::cout << std::endl << "priority sum part 2: " << prioritySumPart2 << std::endl;

  return 0;
}