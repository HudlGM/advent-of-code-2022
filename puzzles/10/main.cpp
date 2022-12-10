#include <fstream>
#include <iostream>
#include <queue>

const std::string FILENAME_EXAMPLE{"puzzles/10/example.txt"};
const std::string FILENAME_INPUT{"puzzles/10/input.txt"};

const std::string INSTRUCTION_ADDX{"addx"};

int sumOfInterestingSignalStrengths(const std::string &filename);

int sumOfInterestingSignalStrengths(const std::string &filename)
{
  int sum = 0;

  int cycle = 1;
  int x = 1;
  std::queue<int> add;

  std::ifstream in(filename);
  std::string l; // line
  std::getline(in, l);
  while (cycle <= 220) {
    if (((cycle - 20) % 40) == 0) {
#ifdef VERBOSE
      std::cout << "[debug] on cycle " << cycle << " x is " << x << ", added signal strength of " << (cycle * x) << ", sum is now " << sum << std::endl;
#endif
      sum += (cycle * x);
    }

    if (add.empty()) {
      // read next instruction
      if (in) {
        if (l.find(INSTRUCTION_ADDX) != std::string::npos) {
          add.push(std::stoi(l.substr(INSTRUCTION_ADDX.size()+1)));
        } // else, no-op assumed

        std::getline(in, l);
      }
    } else {
      // finish add operation
      x += add.front();
      add.pop();
    }

    cycle++;
  }

  return sum;
}

int main()
{
  assert(sumOfInterestingSignalStrengths(FILENAME_EXAMPLE) == 13140);

  std::cout << "sum of interesting signal strengths: " << sumOfInterestingSignalStrengths(FILENAME_INPUT) << std::endl;

  return 0;
}