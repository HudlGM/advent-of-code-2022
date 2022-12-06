#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

const char CHAR_SPACE{' '};
const char CHAR_LEFT_SQUARE_BRACKET{'['};

void reverseStack(std::deque<char> &s)
{
  std::deque<char> temp;

  while (s.empty() == false) {
    temp.push_front(s.front());
    s.pop_front();
  }

  s = temp;
}

void doMoveOperation(const std::string &op, std::vector<std::deque<char>> &stacks)
{
  // "move n from src to dst"
  //      0 1    2   3  4

  const size_t s0 = op.find(CHAR_SPACE);
  const size_t s1 = op.find(CHAR_SPACE, s0+1);
  const size_t s2 = op.find(CHAR_SPACE, s1+1);
  const size_t s3 = op.find(CHAR_SPACE, s2+1);
  const size_t s4 = op.find(CHAR_SPACE, s3+1);

  const int n = std::stoi(op.substr(s0+1,s1));
  const int src = std::stoi(op.substr(s2+1,s3));
  const int dst = std::stoi(op.substr(s4));

#ifdef VERBOSE
  std::cout << "Moving " << n << " from " << src << " to " << dst << std::endl;
#endif

  for (int i=0; i < n; ++i) {
    stacks.at(dst-1).push_front(stacks.at(src-1).front());
    stacks.at(src-1).pop_front();
  }
}

int main()
{
  std::ifstream in("puzzles/5/input.txt");

  std::vector<std::deque<char>> stacks;

  std::string l; // line

  std::getline(in, l);

  // build stack representation upside-down
  while (in) {
    if (l.empty()) {
      // this is the break between the stack representation and move operations, no-op
    } else if (l.at(0) == CHAR_LEFT_SQUARE_BRACKET) {
      // this is part of the stack representation
      size_t pos = 0;
      size_t currentStack = 0;
      while (pos < l.size()) {
        if ((pos + 3) <= l.size()) {
          if (stacks.size() < (currentStack+1)) {
            // initialize this stack
            stacks.push_back(std::deque<char>());
          }

          // consume up to four characters, second character may be item
          const char item = l.at(pos + 1);
          pos += 4;

          if (item != CHAR_SPACE) {
            stacks[currentStack].push_front(item);
          }

          currentStack++;
        } else {
          break;
        }
      }
    } else if (l.at(0) == CHAR_SPACE) {
      // this is the stack labels part of the stack representation

      // flip stack representation
      for (std::deque<char> &s : stacks) {
        reverseStack(s);
      }

#ifdef VERBOSE
      std::cout << "Stacks before moves:" << std::endl;
      int i = 1;
      for (std::deque<char> &s : stacks) {
        std::cout << i++ << ": ";
        for (const char &c : s) {
          std::cout << c;
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
#endif
    } else if (l.at(0) != CHAR_LEFT_SQUARE_BRACKET) {
      // this is a move operation
      doMoveOperation(l, stacks);
    }

    std::getline(in, l);
  }

  std::cout << "Tops of stacks: ";
  for (std::deque<char> &s : stacks) {
    std::cout << s.front();
  }
  std::cout << std::endl;

  return 0;
}