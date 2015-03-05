#include <string>
#include <iostream>
#include <memory>

using namespace std;

struct fatal_codestyle_error
{
  string message;
};

// ASCII Strings only !!!
// Deprecated function!!!
void RemoveDups(char *pStr)
{
  if (!pStr)
    throw fatal_codestyle_error{ "You should not use nullptr like that" };
  if (pStr[0] == 0)
    return;

  while (pStr[0] != pStr[1])
    if (pStr++[1] == 0) // Litle hacky, just showing you that i know what im doing.
      return;

  int shift = 1;
  while (true)
  {
    if (!pStr[shift])
    {
      pStr[1] = pStr[shift];
      return;
    }

    while (*pStr == pStr[shift])
      shift++;
    pStr[1] = pStr[shift];
    pStr++;
  }
}

auto main()
{
  const auto example_size = 100;
  unique_ptr<char[]> test_string = make_unique<char[]>(example_size);
  cin.getline(test_string.get(), example_size);
  RemoveDups(test_string.get());

  // Out of task, we COULD use that COSTLY copying.
  cout << (string)test_string.get();
}