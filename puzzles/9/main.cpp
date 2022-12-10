#include <fstream>
#include <iostream>
#include <vector>
#include <set>

const std::string FILENAME_EXAMPLE_1{"puzzles/9/example1.txt"};
const std::string FILENAME_EXAMPLE_2{"puzzles/9/example2.txt"};
const std::string FILENAME_INPUT{"puzzles/9/input.txt"};

const std::pair<int,int> s = { 0,  0}; // starting position
const std::pair<int,int> U = { 0,  1}; // up
const std::pair<int,int> D = { 0, -1}; // down
const std::pair<int,int> L = {-1,  0}; // left
const std::pair<int,int> R = { 1,  0}; // right
const std::pair<int,int> n = { 0,  0}; // no move

size_t countPositionsVisitedByTail(const std::string &filename, const size_t &knotCount);
void move(const std::pair<int,int> &d, std::vector<std::pair<int,int>> &knots);

size_t countPositionsVisitedByTail(const std::string &filename, const size_t &knotCount)
{
  std::set<std::pair<int,int>> positionsVisitedByTail = { s };

  std::vector<std::pair<int,int>> knots(knotCount, s); // current positions of knots

  std::ifstream in(filename);
  std::string l; // line
  std::getline(in, l);
  while (in) {
    std::pair<int,int> d; // direction

    switch(l.at(0)) {
      case 'U':
        d = U;
        break;

      case 'D':
        d = D;
        break;

      case 'L':
        d = L;
        break;

      case 'R':
        d = R;
        break;

      default:
        d = n;
    }

    const int steps = std::stoi(l.substr(2));

    for (int i=0; i < steps; ++i) {
      move(d, knots);
      positionsVisitedByTail.insert(knots.back());
    }

    std::getline(in, l);
  }

#ifdef VERBOSE
  std::cout << "START" << std::endl;
  for (const std::pair<int,int> &p : positionsVisitedByTail) {
    std::cout << "(" << p.first << "," << p.second << ")" << std::endl;
  }
  std::cout << "END - " << positionsVisitedByTail.size() << std::endl;
#endif

  return positionsVisitedByTail.size();
}

void move(const std::pair<int,int> &d, std::vector<std::pair<int,int>> &knots)
{
  if (knots.size() == 0) {
    return;
  }

  knots.front().first += d.first;
  knots.front().second += d.second;

  std::pair<int,int> lastMove = d;

  for (size_t i=1; i < knots.size(); ++i) {
    std::pair<int,int> &prev = knots.at(i-1);
    std::pair<int,int> &next = knots.at(i);

    const std::pair<int,int> startingPositionOfNext = next;

    const int absoluteDistanceX = std::abs(prev.first-next.first);
    const int absoluteDistanceY = std::abs(prev.second-next.second);

    if (
      (absoluteDistanceX <= 1) &&
      (absoluteDistanceY <= 1)
    ) {
      // prev and next already touching, no more moves required
      return;
    }

    if (absoluteDistanceX == 2) {
      next.first += lastMove.first; // move in same direction

      if (absoluteDistanceY == 1) {
        next.second = prev.second; // also move diagonally
      }
    }

    if (absoluteDistanceY == 2) {
      next.second += lastMove.second; // move in same direction

      if (absoluteDistanceX == 1) {
        next.first = prev.first; // also move diagonally
      }
    }

    lastMove.first = next.first - startingPositionOfNext.first;
    lastMove.second = next.second - startingPositionOfNext.second;
  }
}

int main()
{
  assert(countPositionsVisitedByTail(FILENAME_EXAMPLE_1, 2) == 13);
  assert(countPositionsVisitedByTail(FILENAME_EXAMPLE_2, 10) == 36);

  std::cout << "positions visited by tail (2 knots): " << countPositionsVisitedByTail(FILENAME_INPUT, 2) << std::endl;
  std::cout << "positions visited by tail (10 knots): " << countPositionsVisitedByTail(FILENAME_INPUT, 10) << std::endl;

  return 0;
}