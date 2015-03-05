#include "list.h"
#include <random>

using namespace std;

ListIterator &ListIterator::operator++()
{
  if (!containee)
    throw List::nullptr_pointer();
  containee = containee->next;
  return *this;
}

ListIterator::operator ListNode *() const
{
  return containee;
}


ListIterator List::begin() const
{
  return{ head };
}

ListIterator List::end() const
{
  return{ nullptr };
}

std::ostream &operator<<(std::ostream &stream, const List &list)
{
  int i = 0;
  stream << "==BEGIN LIST DEBUG INFO. SIZE [" << list.count << "]" << endl;
  for (auto &node : list)
  {
    stream << "[" << i++ << "] " << node.data << " -> ";
    if (!node.rand)
      stream << "NULL";
    else
      stream << node.rand->data;
    // Hey, i just realised that you should recheck references on every element remove
    // which causing O(N) for erasing(iterator). :(((
    stream << endl;
  }
  stream << "==END LIST DEBUG INFO" << endl;
  return stream;
}

void List::_DebugSetRandPointers()
{
  if (head == nullptr)
    return;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> offsets(0, count - 1);

  auto GetPointer = [this](int offset)
  {
    auto *p = head;

    while (offset-- > 0)
      p = p->next;
    return p;
  };

  std::cout << "ROLLIN: ";
  for (auto &node : *this)
  {
    int roll = offsets(generator);
    std::cout << roll << " ";
    node.rand = GetPointer(roll);
  }
  std::cout << std::endl;
}