#include <fstream>
#include <iostream>

const char ELF_SEP{','};
const char RANGE_SEP{'-'};

struct Range
{
  Range()
    : start{0}
    , end{0}
  {
  }

  int start;
  int end;
};

void parseSectionRanges(const std::string &l, Range &e1, Range &e2)
{
#ifdef VERBOSE
  std::cout << "Line " << l << std::endl;
#endif

  const std::string r1 = l.substr(0, l.find(ELF_SEP));
#ifdef VERBOSE
  std::cout << "  First elf's section range (" << r1 << ") is ";
#endif
  e1.start = std::stoi(r1.substr(0, r1.find(RANGE_SEP)));
  e1.end = std::stoi(r1.substr(r1.find(RANGE_SEP)+1));
#ifdef VERBOSE
  std::cout << e1.start << " to " << e1.end << std::endl;
#endif

  const std::string r2 = l.substr(l.find(ELF_SEP)+1);
#ifdef VERBOSE
  std::cout << "  Second elf's section range (" << r2 << ") is ";
#endif
  e2.start = std::stoi(r2.substr(0, r2.find(RANGE_SEP)));
  e2.end = std::stoi(r2.substr(r2.find(RANGE_SEP)+1));
#ifdef VERBOSE
  std::cout << e2.start << " to " << e2.end << std::endl;
#endif
}

bool hasFullyContainedRange(const Range &e1, const Range &e2)
{
  if (
    ((e2.start >= e1.start) && (e2.end <= e1.end)) ||
    ((e1.start >= e2.start) && (e1.end <= e2.end))
  ) {
#ifdef VERBOSE
    std::cout << "  Has fully contained range";
#endif
    return true;
  }

#ifdef VERBOSE
    std::cout << "  Does not have fully contained range";
#endif
  return false;
}

bool isContainedInRange(const int &x, const Range &r)
{
  return (x >= r.start) && (x <= r.end);
}

bool hasAnyOverlap(const Range &e1, const Range &e2)
{
  for (int i=e1.start; i <= e1.end; ++i) {
    if (isContainedInRange(i, e2)) {
#ifdef VERBOSE
      std::cout << std::endl << "  Has some overlap";
#endif
      return true;
    }
  }

#ifdef VERBOSE
  std::cout << std::endl << "  Does not have any overlap";
#endif
  return false;
}

int main()
{
  std::ifstream in("puzzles/4/input.txt");

  std::string l; // line
  Range e1; // first elf in pair
  Range e2; // second elf in pair

  int countPairsWithFullyContainedRange = 0;
  int countPairsWithAnyOverlap = 0;

  std::getline(in, l);

  while (in) {
    parseSectionRanges(l, e1, e2);

    if (hasFullyContainedRange(e1, e2)) {
      countPairsWithFullyContainedRange++;
      countPairsWithAnyOverlap++; // fully contained range does include overlap
    } else if (hasAnyOverlap(e1, e2)) {
      countPairsWithAnyOverlap++;
    }

#ifdef VERBOSE
    std::cout << std::endl;
#endif

    std::getline(in, l);
  }

  std::cout << std::endl << "Pairs with fully contained range: " << countPairsWithFullyContainedRange << std::endl;
  std::cout << std::endl << "Pairs with any overlap: " << countPairsWithAnyOverlap << std::endl;

  return 0;
}