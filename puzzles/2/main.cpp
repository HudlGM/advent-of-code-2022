#include <fstream>
#include <iostream>

enum class Type
{
  ROCK,
  PAPER,
  SCISSORS,
};

enum class Outcome
{
  LOSE,
  DRAW,
  WIN,
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

Outcome readDesiredOutcome(const char &c)
{
  if (c == 'X') {
#ifdef VERBOSE
      std::cout << c << "=Lose, ";
#endif
    return Outcome::LOSE;
  }

  if (c == 'Y') {
#ifdef VERBOSE
      std::cout << c << "=Draw, ";
#endif
    return Outcome::DRAW;
  }

#ifdef VERBOSE
      std::cout << c << "=Win, ";
#endif
  return Outcome::WIN;
}

Type selectTypeForDesiredOutcome(const Type &opponent, const Outcome &desiredOutcome)
{
  if (desiredOutcome == Outcome::DRAW) {
    return opponent;
  }

  if (desiredOutcome == Outcome::LOSE) {
    if (opponent == Type::ROCK) {
      return Type::SCISSORS;
    }

    if (opponent == Type::PAPER) {
      return Type::ROCK;
    }

    return Type::PAPER;
  }

  // otherwise, win

  if (opponent == Type::ROCK) {
    return Type::PAPER;
  }

  if (opponent == Type::PAPER) {
    return Type::SCISSORS;
  }

  return Type::ROCK;
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

int playerTotalForMatch(const Type &opponent, const Type &player)
{
  if (opponent == player) {
#ifdef VERBOSE
      std::cout << "Draw, ";
#endif
    return 3;
  }

  if (
    ((player == Type::ROCK    ) && (opponent == Type::SCISSORS)) ||
    ((player == Type::SCISSORS) && (opponent == Type::PAPER   )) ||
    ((player == Type::PAPER   ) && (opponent == Type::ROCK    ))
  ) {
#ifdef VERBOSE
      std::cout << "Win, ";
#endif
    return 6;
  }

#ifdef VERBOSE
      std::cout << "Loss, ";
#endif
  return 0;
}

int computeScore(const Type &opponent, const Type &player)
{
  return typeToPoints(player) + playerTotalForMatch(opponent, player);
}

int main()
{
  std::ifstream in("puzzles/2/input.txt");

  int totalUsingPart1Strategy = 0;
  int totalUsingPart2Strategy = 0;

  std::string s;

  std::getline(in, s);

  while (in) {
    const Type o = readType(s.at(0));
    const Type p = readType(s.at(2)); // part 1 assumes second column is player type

    const Outcome desiredOutcome = readDesiredOutcome(s.at(2)); // part 2 understands second column is desired outcome

#ifdef VERBOSE
      std::cout << "(strategy 1) ";
#endif
    totalUsingPart1Strategy += computeScore(o,p);

#ifdef VERBOSE
      std::cout << "(Strategy 2) ";
#endif
    totalUsingPart2Strategy += computeScore(o,selectTypeForDesiredOutcome(o, desiredOutcome));

#ifdef VERBOSE
      std::cout << std::endl;
#endif

    std::getline(in, s);
  }

  std::cout << std::endl << "totalUsingPart1Strategy: " << totalUsingPart1Strategy << std::endl;
  std::cout << std::endl << "totalUsingPart2Strategy: " << totalUsingPart2Strategy << std::endl;

  return 0;
}