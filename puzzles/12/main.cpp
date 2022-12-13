#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

const std::string FILENAME_EXAMPLE{"puzzles/12/example.txt"};
const std::string FILENAME_INPUT{"puzzles/12/input.txt"};

const char START{'S'};
const char END{'E'};
const char MOVED_UP{'^'};
const char MOVED_DOWN{'V'}; // Note: lowercase 'v' is an elevation...
const char MOVED_LEFT{'<'};
const char MOVED_RIGHT{'>'};
const char UNVISITED{'.'};
const char ELEVATION_START{'a'};
const char ELEVATION_END{'z'};
const long MAX_CLIMB{1};

enum class Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

struct Point
{
  Point(const size_t &x_, const size_t &y_)
    : x{x_}
    , y{y_}
  {

  }

  ~Point()
  {

  }

  Point up() const {
    return Point(x, y-1);
  }

  Point down() const {
    return Point(x, y+1);
  }

  Point left() const {
    return Point(x-1, y);
  }

  Point right() const {
    return Point(x+1, y);
  }

  friend bool operator==(const Point &a, const Point &b)
  {
    return (a.x == b.x) && (a.y == b.y);
  }

  friend bool operator!=(const Point &a, const Point &b)
  {
    return !(a==b);
  }

  size_t x;
  size_t y;
};

char get(const std::vector<std::vector<char>> &grid, const Point &p);
bool canMove(const std::vector<std::vector<char>> &grid, const Point &s, const Direction &d);
std::vector<std::deque<Point>> findAllPossiblePaths(const std::vector<std::vector<char>> &grid, const Point &s, const Point &e);
size_t shortestPath(const std::string &filename);

char get(const std::vector<std::vector<char>> &grid, const Point &p)
{
  return grid.at(p.y).at(p.x);
}

bool canMove(const std::vector<std::vector<char>> &grid, const Point &s, const Direction &d)
{
  const char currentElevation = get(grid, s);
  char newElevation = UNVISITED;

  switch (d) {
    case Direction::UP:
      {
        if (s.y <= 0) {
          return false;
        }

        newElevation = get(grid, s.up());
      }
      break;

    case Direction::DOWN:
      {
        if (s.y >= (grid.size() - 1)) {
          return false;
        }

        newElevation = get(grid, s.down());
      }
      break;

    case Direction::LEFT:
      {
        if (s.x <= 0) {
          return false;
        }

        newElevation = get(grid, s.left());
      }
      break;

    case Direction::RIGHT:
      {
        if (s.x >= (grid.back().size() - 1)) {
          return false;
        }

        newElevation = get(grid, s.right());
      }
      break;

    default:
      {
        // unhandled direction
        return false;
      }
  }

  if ((newElevation < ELEVATION_START) || (newElevation > ELEVATION_END)) {
    // comparison not possible
    return false;
  }

  return ((newElevation - currentElevation) <= MAX_CLIMB);
}

std::vector<std::deque<Point>> findAllPossiblePaths(const std::vector<std::vector<char>> &grid, const Point &s, const Point &e)
{
  std::vector<std::deque<Point>> possiblePaths;

#ifdef VERBOSE
  static size_t iteration = std::numeric_limits<size_t>::max();
  iteration++;
  if ((iteration % 100000) == 0) {
    std::cout << "findAllPossiblePaths iteration = " << iteration << std::endl;

    std::cout << std::endl;
    for (size_t row=0; row < grid.size(); ++row) {
      for (size_t col=0; col < grid.back().size(); ++col) {
        std::cout << grid.at(row).at(col);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
#endif

  // try up
  if (canMove(grid, s, Direction::UP)) {
    if (s.up() == e) {
      // stop
      possiblePaths.push_back({s.up()});
    } else {
      // keep going
      std::vector<std::vector<char>> gridCopy = grid;
      gridCopy.at(s.y).at(s.x) = MOVED_UP;

      std::vector<std::deque<Point>> possiblePathsAfterUp = findAllPossiblePaths(gridCopy, s.up(), e);

      for (std::deque<Point> &paths : possiblePathsAfterUp) {
        paths.push_front(s);
      }

      possiblePaths.insert(possiblePaths.end(), possiblePathsAfterUp.begin(), possiblePathsAfterUp.end());
    }
  }

  // try down
  if (canMove(grid, s, Direction::DOWN)) {
    if (s.down() == e) {
      // stop
      possiblePaths.push_back({s.down()});
    } else {
      // keep going
      std::vector<std::vector<char>> gridCopy = grid;
      gridCopy.at(s.y).at(s.x) = MOVED_DOWN;

      std::vector<std::deque<Point>> possiblePathsAfterDown = findAllPossiblePaths(gridCopy, s.down(), e);

      for (std::deque<Point> &paths : possiblePathsAfterDown) {
        paths.push_front(s);
      }

      possiblePaths.insert(possiblePaths.end(), possiblePathsAfterDown.begin(), possiblePathsAfterDown.end());
    }
  }

  // try left
  if (canMove(grid, s, Direction::LEFT)) {
    if (s.left() == e) {
      // stop
      possiblePaths.push_back({s.left()});
    } else {
      // keep going

      std::vector<std::vector<char>> gridCopy = grid;
      gridCopy.at(s.y).at(s.x) = MOVED_LEFT;

      std::vector<std::deque<Point>> possiblePathsAfterLeft = findAllPossiblePaths(gridCopy, s.left(), e);

      for (std::deque<Point> &paths : possiblePathsAfterLeft) {
        paths.push_front(s);
      }

      possiblePaths.insert(possiblePaths.end(), possiblePathsAfterLeft.begin(), possiblePathsAfterLeft.end());
    }
  }

  // try right
  if (canMove(grid, s, Direction::RIGHT)) {
    if (s.right() == e) {
      // stop
      possiblePaths.push_back({s.right()});
    } else {
      // keep going
      std::vector<std::vector<char>> gridCopy = grid;
      gridCopy.at(s.y).at(s.x) = MOVED_RIGHT;

      std::vector<std::deque<Point>> possiblePathsAfterRight = findAllPossiblePaths(gridCopy, s.right(), e);

      for (std::deque<Point> &paths : possiblePathsAfterRight) {
        paths.push_front(s);
      }

      possiblePaths.insert(possiblePaths.end(), possiblePathsAfterRight.begin(), possiblePathsAfterRight.end());
    }
  }

  return possiblePaths;
}

size_t shortestPath(const std::string &filename)
{
  std::vector<std::vector<char>> grid;
  grid.push_back(std::vector<char>());

  Point s(0,0); // start (row, col)
  Point e(0,0); // end (row, col)

  std::ifstream in(filename);
  char c = in.get();
  while (in) {
    if(c == '\n') {
      grid.push_back(std::vector<char>());
    } else if (c == START) {
      s.y = grid.size() - 1;
      s.x = grid.back().size();

      grid.back().push_back(ELEVATION_START);
    } else if (c == END) {
      e.y = grid.size() - 1;
      e.x = grid.back().size();

      grid.back().push_back(ELEVATION_END);
    } else {
      grid.back().push_back(c);
    }

    c = in.get();
  }

#ifdef VERBOSE
  std::cout << "[debug] grid has " << grid.size() << " rows and " << grid.back().size() << " columns" << std::endl;
  std::cout << "[debug] start is at (" << s.x << ", " << s.y << ")" << std::endl;
  std::cout << "[debug] end is at (" << e.x << ", " << e.y << ")" << std::endl;
  std::cout << std::endl;
  for (size_t row=0; row < grid.size(); ++row) {
    for (size_t col=0; col < grid.back().size(); ++col) {
      std::cout << grid.at(row).at(col);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
#endif

  std::vector<std::deque<Point>> possiblePaths = findAllPossiblePaths(grid, s, e);

#ifdef VERBOSE
  std::cout << "[debug] # of possible paths " << possiblePaths.size() << std::endl;
#endif

  size_t shortest = std::numeric_limits<size_t>::max();
  for (std::deque<Point> &path : possiblePaths) {
    if (path.size() < shortest) {
      shortest = path.size();
    }
  }

#ifdef VERBOSE
  std::cout << "[debug] shortest path " << shortest << std::endl;
#endif

  return shortest;
}

int main()
{
  assert(shortestPath(FILENAME_EXAMPLE) == 31);

  std::cout << "shortest path:" << shortestPath(FILENAME_INPUT) << std::endl;

  return 0;
}