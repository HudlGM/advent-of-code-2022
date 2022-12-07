#include <fstream>
#include <iostream>
#include <deque>
#include <unordered_set>

size_t uniqueSize(const std::deque<char> &d)
{
  // this seems terribly inefficient
  std::unordered_set<char> s;
  for (const char &c : d) {
    s.insert(c);
  }
  return s.size();
}

int main()
{
  std::ifstream in("puzzles/6/input.txt");

  std::string l; // line

  int count = 0;
  std::deque<char> seq;

  seq.push_back(in.get());

  while (in) {
    count++;

    while (seq.size() > 4) {
      seq.pop_front();
    }

    if (uniqueSize(seq) == 4) {
      break;
    }

    seq.push_back(in.get());
  }

  std::cout << "count: " << count << std::endl;

  return 0;
}