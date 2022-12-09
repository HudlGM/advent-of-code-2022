#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

size_t visibleTrees(const std::vector<std::vector<int>> &forest);
size_t visibleTrees(const std::string &filename);

size_t visibleTrees(const std::vector<std::vector<int>> &forest)
{
  const size_t H = forest.size(); // grid height
  const size_t W = forest.back().size(); // grid width

  if ((H <= 2) && (W <= 2)) {
    return H * W;
  }

  size_t result = (2 * H) + (2 * W) - 4;

  for (size_t r=1; r < (H-1); ++r) { // rows
    for (size_t c=1; c < (W-1); ++c) { // cols
      const int currentTreeHeight = forest.at(r).at(c);

      // up
      {
        bool visibleUp = true;

        for (size_t i=(r-1); i != std::numeric_limits<size_t>::max(); --i) {
          if (currentTreeHeight <= forest.at(i).at(c)) {
            visibleUp = false;
            break;
          }
        }

        if (visibleUp) {
          result++;
          continue;
        }
      }

      // down
      {
        bool visibleDown = true;

        for (size_t i=(r+1); i < H; ++i) {
          if (currentTreeHeight <= forest.at(i).at(c)) {
            visibleDown = false;
            break;
          }
        }

        if (visibleDown) {
          result++;
          continue;
        }
      }

      // left
      {
        bool visibleLeft = true;

        for (size_t i=(c-1); i != std::numeric_limits<size_t>::max(); --i) {
          if (currentTreeHeight <= forest.at(r).at(i)) {
            visibleLeft = false;
            break;
          }
        }

        if (visibleLeft) {
          result++;
          continue;
        }
      }

      // right
      {
        bool visibleRight = true;

        for (size_t i=(c+1); i < W; ++i) {
          if (currentTreeHeight <= forest.at(r).at(i)) {
            visibleRight = false;
            break;
          }
        }

        if (visibleRight) {
          result++;
          continue;
        }
      }
    }
  }

  return result;
}

size_t visibleTrees(const std::string &filename)
{
  std::ifstream in(filename);

  std::vector<std::vector<int>> forest;

  std::string l; // line

  std::getline(in, l);

  while (in) {
    forest.push_back({});

    for (const char &tree : l) {
      forest.back().push_back(static_cast<int>(tree));
    }

    std::getline(in, l);
  }

  return visibleTrees(forest);
}

int main()
{
  assert(visibleTrees("puzzles/8/example.txt") == 21);

  std::cout << "visible trees: " << visibleTrees("puzzles/8/input.txt") << std::endl;

  return 0;
}