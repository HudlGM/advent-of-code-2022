#include <fstream>
#include <iostream>

char findSharedItem(const std::string &c1, const std::string &c2)
{
  for (const char &c : c1) {
    if (c2.find(c) != std::string::npos) {
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
  std::ifstream in("3.in");

  std::string s; // rucksack contents
  std::string c1; // compartment 1
  std::string c2; // compartment 2

  int prioritySum = 0;

  std::getline(in, s);

  while (in) {
    // first half first compartment
    c1 = s.substr(0,s.length()/2);

    // second half second compartment
    c2 = s.substr(s.length()/2);

    prioritySum += getPriority(findSharedItem(c1,c2));

    std::getline(in, s);
  }

  std::cout << std::endl << "priority sum: " << prioritySum << std::endl;

  return 0;
}