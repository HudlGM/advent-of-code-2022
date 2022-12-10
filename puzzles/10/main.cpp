#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>

const std::string FILENAME_EXAMPLE{"puzzles/10/example.txt"};
const std::string FILENAME_INPUT{"puzzles/10/input.txt"};

const std::string INSTRUCTION_ADDX{"addx"};

const int S{3}; // sprite width
const int W{40}; // CRT width
const int H{6}; // CRT height

const char ON{'#'};
const char OFF{'.'};

const std::string EXAMPLE_OUTPUT{
  "##..##..##..##..##..##..##..##..##..##..\n"
  "###...###...###...###...###...###...###.\n"
  "####....####....####....####....####....\n"
  "#####.....#####.....#####.....#####.....\n"
  "######......######......######......####\n"
  "#######.......#######.......#######.....\n"
};

int sumOfInterestingSignalStrengths(const std::string &filename);
std::string drawScreen(const std::string &filename);

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

std::string drawScreen(const std::string &filename)
{
  std::string pixels;

  int cycle = 1;
  int x = 1; // horizontal position of middle of sprite
  std::queue<int> add;

  std::ifstream in(filename);
  std::string l; // line
  std::getline(in, l);
  while (cycle <= (W*H)) {
    const int cursorPos = ((cycle-1) % W);
    if (std::abs(x - cursorPos) <= 1) {
      pixels += ON;
    } else {
      pixels += OFF;
    }

    if (cursorPos == (W-1)) {
      pixels += '\n';
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

#ifdef VERBOSE
  std::cout << "[debug] pixels " << std::endl << pixels << std::endl;
#endif

  return pixels;
}

int main()
{
  assert(sumOfInterestingSignalStrengths(FILENAME_EXAMPLE) == 13140);
  assert(drawScreen(FILENAME_EXAMPLE) == EXAMPLE_OUTPUT);

  std::cout << "sum of interesting signal strengths: " << sumOfInterestingSignalStrengths(FILENAME_INPUT) << std::endl;
  std::cout << "screen:" << std::endl << drawScreen(FILENAME_INPUT) << std::endl;

  return 0;
}