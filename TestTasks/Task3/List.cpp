#include "list.h"
#include <memory>

using namespace std;

// Yeah, i know about references. But since c++11 we have
// goodlike 'move sematics'.
List::List(std::initializer_list<std::string> list)
{
  for (auto &data : list)
  {
    unique_ptr<ListNode> tmp = make_unique<ListNode>();
    tmp->data = data;

    if (0)
    {
      static_assert(noexcept(PushBack(tmp.release())), "List::PushBack should be exception safe. Or potential exception occure");
      PushBack(tmp.release());
    }
    else
    { // just example
      PushBack(tmp.get());
      tmp.release();
    }
  }
}