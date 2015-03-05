#include "list.h"
#include <sstream>

auto main()
{
  List test =
  {
    "HEY",
    "I WANT",
    "WORK WITH",
    "YOU !!!",
    ":)))"
  };

  test._DebugSetRandPointers();
  std::cout << "Initial state" << std::endl << test << std::endl;

  std::stringstream ss;
  test.Serialize(ss);

  std::cout << "SERIALIZED: " << std::endl << ss.str() << std::endl;

  List tset = {};
  tset.Deserialize(ss);

  std::cout << "DESERIALIZED: " << std::endl << tset;
}