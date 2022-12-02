#include <iostream>
#include <fstream>

int main()
{
  std::ifstream in("1.in");

  int count = 0;
  int caloriesHeldByCurrentElf = 0;
  int elfWithMostCalories = 0;
  int mostCaloriesHeldByElf = 0;

  std::string s;
  while (in) {
    std::getline(in, s);

    if (s.empty() || in.fail()) {
#ifdef VERBOSE
      std::cout << "Elf " << ++count << " is holding " << caloriesHeldByCurrentElf << " calories" << std::endl;
#endif
      if (caloriesHeldByCurrentElf > mostCaloriesHeldByElf) {
        elfWithMostCalories = count;
        mostCaloriesHeldByElf = caloriesHeldByCurrentElf;
      }

      caloriesHeldByCurrentElf = 0;
    } else {
#ifdef VERBOSE
      std::cout << caloriesHeldByCurrentElf << "+" << std::stoi(s);
#endif
      caloriesHeldByCurrentElf += std::stoi(s);
#ifdef VERBOSE
      std::cout << "=" << caloriesHeldByCurrentElf << std::endl;
#endif
    }
  }

  std::cout << std::endl << "Elf " << elfWithMostCalories << " is holding the most calories (" << mostCaloriesHeldByElf << ")" << std::endl << std::endl;

  return 0;
}