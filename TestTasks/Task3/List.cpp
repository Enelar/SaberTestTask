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

    if (1)
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

void List::PushBack(ListNode *that) noexcept
{
  if (!that)
    return; // I think you shouldn't do this <3

  count++;
  that->rand = nullptr; // just in case.

  if (!tail)
  {
    head = tail = that->next = that->prev = that;
    return;
  }

  that->next = tail->next;
  that->next->prev = that;

  that->prev = tail;
  tail->next = that;

  tail = that;
}