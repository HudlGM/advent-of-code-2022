#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

const std::string FILENAME_EXAMPLE{"puzzles/8/example.txt"};
const std::string FILENAME_INPUT{"puzzles/8/input.txt"};

void parse(const std::string &filename, std::vector<std::vector<int>> &forestOut);
size_t visibleTrees(const std::vector<std::vector<int>> &forest);
size_t visibleTrees(const std::string &filename);
size_t bestScenicScore(const std::vector<std::vector<int>> &forest);
size_t bestScenicScore(const std::string &filename);

void parse(const std::string &filename, std::vector<std::vector<int>> &forestOut)
{
  std::ifstream in(filename);

  std::string l; // line

  std::getline(in, l);

  while (in) {
    forestOut.push_back({});

    for (const char &tree : l) {
      forestOut.back().push_back(static_cast<int>(tree));
    }

    std::getline(in, l);
  }
}

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
  std::vector<std::vector<int>> forest;

  parse(filename, forest);

  return visibleTrees(forest);
}

size_t bestScenicScore(const std::vector<std::vector<int>> &forest)
{
  const size_t H = forest.size(); // grid height
  const size_t W = forest.back().size(); // grid width

  // Note: Because scenic score multiplies visible trees in all directions,
  // edge trees have a score of zero.

  if ((H <= 2) && (W <= 2)) {
    return 0;
  }

  size_t highest = 0;

  for (size_t r=1; r < (H-1); ++r) { // rows
    for (size_t c=1; c < (W-1); ++c) { // cols
      const int currentTreeHeight = forest.at(r).at(c);

      size_t ssUp = 0; // scenic score up
      for (size_t i=(r-1); i != std::numeric_limits<size_t>::max(); --i) {
        ssUp++;

        if (currentTreeHeight <= forest.at(i).at(c)) {
          break;
        }
      }

      size_t ssDown = 0; // scenic score down
      for (size_t i=(r+1); i < H; ++i) {
        ssDown++;

        if (currentTreeHeight <= forest.at(i).at(c)) {
          break;
        }
      }

      size_t ssLeft = 0; // scenic score left
      for (size_t i=(c-1); i != std::numeric_limits<size_t>::max(); --i) {
        ssLeft++;

        if (currentTreeHeight <= forest.at(r).at(i)) {
          break;
        }
      }

      size_t ssRight = 0; // scenic score right
      for (size_t i=(c+1); i < W; ++i) {
        ssRight++;

        if (currentTreeHeight <= forest.at(r).at(i)) {
          break;
        }
      }

      const size_t ss = ssUp * ssDown * ssLeft * ssRight; // scenic score of current tree

      if (ss > highest) {
        highest = ss;
      }
    }
  }

  return highest;
}

size_t bestScenicScore(const std::string &filename)
{
  std::vector<std::vector<int>> forest;

  parse(filename, forest);

  return bestScenicScore(forest);
}

int main()
{
  assert(visibleTrees(FILENAME_EXAMPLE) == 21);
  assert(bestScenicScore(FILENAME_EXAMPLE) == 8);

  std::cout << "visible trees: " << visibleTrees(FILENAME_INPUT) << std::endl;
  std::cout << "best scenic score: " << bestScenicScore(FILENAME_INPUT) << std::endl;

  return 0;
}