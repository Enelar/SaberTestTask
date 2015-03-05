#include <iostream>
#include <string>
using namespace std;

auto main()
{
  cout << "TASK 1" << endl;

  int num;
  cin >> num;

  auto still_saw_only_zeros = true;
  const auto bits_in_byte = 8;
  int current_bit = sizeof(num) * 8; // start from left

  while (--current_bit >= 0)
  {
    auto bit = (num >> current_bit) & 1;
    if (!bit && still_saw_only_zeros)
      continue;
    still_saw_only_zeros = false; // it faster than with compare
    cout << (char)('0' + bit);
  }
  cout << endl;
}