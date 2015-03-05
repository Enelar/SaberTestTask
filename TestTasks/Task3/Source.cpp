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

  std::stringstream ss;
  test.Serialize(ss);

  List tset = {};
  tset.Deserialize(ss);
}