#include <fstream>
#include <iostream>

enum class Type
{
  ROCK,
  PAPER,
  SCISSORS,
};

Type readType(const char &c)
{
  if ((c == 'A') || (c == 'X')) {
#ifdef VERBOSE
      std::cout << c << "=Rock, ";
#endif
    return Type::ROCK;
  }

  if ((c == 'B') || (c == 'Y')) {
#ifdef VERBOSE
      std::cout << c << "=Paper, ";
#endif
    return Type::PAPER;
  }

#ifdef VERBOSE
      std::cout << c << "=Scissors, ";
#endif
  return Type::SCISSORS;
}

int typeToPoints(const Type &t)
{
  switch (t) {
    case Type::ROCK:
    {
#ifdef VERBOSE
      std::cout << "1 point for rock, ";
#endif
      return 1;
    }
    case Type::PAPER:
    {
#ifdef VERBOSE
      std::cout << "2 points for paper, ";
#endif
      return 2;
    }
    case Type::SCISSORS:
    default:
    {
#ifdef VERBOSE
      std::cout << "3 points for scissors, ";
#endif
      return 3;
    }
  }
}

int outcomeToPoints(const Type &opponent, const Type &player)
{
  if (opponent == player) {
#ifdef VERBOSE
      std::cout << "Draw" << std::endl;
#endif
    return 3;
  }

  if (
    ((player == Type::ROCK    ) && (opponent == Type::SCISSORS)) ||
    ((player == Type::SCISSORS) && (opponent == Type::PAPER   )) ||
    ((player == Type::PAPER   ) && (opponent == Type::ROCK    ))
  ) {
#ifdef VERBOSE
      std::cout << "Win" << std::endl;
#endif
    return 6;
  }

#ifdef VERBOSE
      std::cout << "Loss" << std::endl;
#endif
  return 0;
}

int computeScore(const Type &opponent, const Type &player)
{
  return typeToPoints(player) + outcomeToPoints(opponent, player);
}

int main()
{
  std::ifstream in("2.in");

  int total = 0;

  std::string s;

  std::getline(in, s);

  while (in) {
    const Type o = readType(s.at(0));
    const Type p = readType(s.at(2));

    total += computeScore(o,p);

    std::getline(in, s);
  }

  std::cout << std::endl << "Total: " << total << std::endl;

  return 0;
}