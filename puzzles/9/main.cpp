#include <fstream>
#include <iostream>
#include <vector>
#include <set>

const std::string FILENAME_EXAMPLE{"puzzles/9/example.txt"};
const std::string FILENAME_INPUT{"puzzles/9/input.txt"};

const std::pair<int,int> s = { 0,  0}; // starting position
const std::pair<int,int> U = { 0,  1}; // up
const std::pair<int,int> D = { 0, -1}; // down
const std::pair<int,int> L = {-1,  0}; // left
const std::pair<int,int> R = { 1,  0}; // right
const std::pair<int,int> n = { 0,  0}; // no move

size_t countPositionsVisitedByTail(const std::string &filename);
void move(const std::pair<int,int> &d, std::pair<int,int> &H, std::pair<int,int> &T);

size_t countPositionsVisitedByTail(const std::string &filename)
{
  std::set<std::pair<int,int>> positionsVisitedByTail = { s };

  std::pair<int,int> T = s; // current position of tail
  std::pair<int,int> H = s; // current position of head

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
      move(d, H, T);
      positionsVisitedByTail.insert(T);
    }

    std::getline(in, l);
  }

  return positionsVisitedByTail.size();
}

void move(const std::pair<int,int> &d, std::pair<int,int> &H, std::pair<int,int> &T)
{
  H.first += d.first;
  H.second += d.second;

  const int absoluteDistanceX = std::abs(H.first-T.first);
  const int absoluteDistanceY = std::abs(H.second-T.second);

  if (
    (absoluteDistanceX <= 1) &&
    (absoluteDistanceY <= 1)
   ) {
    // H and T already touching, no move required
    return;
  }

  if (absoluteDistanceX == 2) {
    T.first += d.first; // move in same direction

    if (absoluteDistanceY == 1) {
      T.second = H.second; // also move diagonally
    }
  } else if (absoluteDistanceY == 2) {
    T.second += d.second; // move in same direction

    if (absoluteDistanceX == 1) {
      T.first = H.first; // also move diagonally
    }
  }
}

int main()
{
  assert(countPositionsVisitedByTail(FILENAME_EXAMPLE) == 13);

  std::cout << "positions visited by tail: " << countPositionsVisitedByTail(FILENAME_INPUT) << std::endl;

  return 0;
}