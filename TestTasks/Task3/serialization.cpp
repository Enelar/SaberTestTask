#include "list.h"
#include <map>
#include <memory>
using namespace std;

namespace
{
  static const string magic_word = "LIST";
  static const int serialize_version = 0;

  static_assert(sizeof(int) == sizeof(ListNode *), "Cannot perform List::Serialization on this machine");
};

void List::Serialize(ostream & stream) const
{
  // I hope that we need speed while we saving state
  // It could be autosave in midgame, while player need PERFORMANCE

  stream << magic_word << serialize_version;
  stream << count;

  if (!head)
    return; // Serialisation finished
  if (!tail)
    throw general_serialzation_failure();

  if (head == tail)
  {
    if (count != 1)
      throw general_serialzation_failure();
    
    stream << reinterpret_cast<int>(head->rand);
    stream << head->data.length() << head->data << 0; // ASCII ONLY!!!
    return;
  }

  auto *current = head;
  auto counter = 0;

  do
  {
    if (counter++ > count)
      throw general_serialzation_failure();
    if (!current)
      throw general_serialzation_failure();

    { // Node serialization code
      const auto &that = *current; // It would be optimised by compiler

      // Store raw pointer for decrypt 'rand' in deserialization state
      // Since sizeof int is equal to void * on most platforms
      stream << reinterpret_cast<int>(&that);
      stream << reinterpret_cast<int>(that.rand);

      stream << that.data.size(); // that faster
      stream << that.data << 0; // ASCII ONLY!!
    }
    
    current = current->next; // Instead of current++
  } while (current != tail);
}

namespace
{
  void DataDeserialize(istream & stream, string & str)
  {
    int data_size;
    stream >> data_size;
    if (data_size < 0)
      throw List::general_serialzation_failure();
    str.resize(data_size);

    stream.getline(&str[0], data_size);
  }
}

void List::Deserialize(istream & stream)
{
  if (head || tail || count)
    throw general_serialzation_failure(); // out of task.

  { // Check that we could deserialize this
    string they_saying;
    they_saying.resize(magic_word.size());
    stream.getline(&they_saying[0], magic_word.size());
    if (they_saying != magic_word)
      throw general_serialzation_failure();

    int version;
    stream >> version;
    if (version != serialize_version)
      throw general_serialzation_failure();
  }
  
  stream >> count;

  if (!count)
    return;

  if (count == 1)
  {
    tail = head = new ListNode; // POTENTIAL MEMLEAK
    head->next = head->prev = head;
    {
      int rand_pointer;
      stream >> rand_pointer;
      head->rand = rand_pointer ? head : nullptr;
    }
    DataDeserialize(stream, head->data);
    return;
  }

  // maps old one into new one
  map<ListNode *, ListNode *> pointers;

  // Data Loading, rand pointers invalid
  for (auto i = 0; i < count; i++)
  {
    unique_ptr<ListNode> tmp = make_unique<ListNode>();
    ListNode &that = *tmp;
    ListNode *old_invalid_pointer;

    stream >> reinterpret_cast<int &>(old_invalid_pointer);
    pointers.insert({ old_invalid_pointer, &that });

    stream >> reinterpret_cast<int &>(that.rand);
    DataDeserialize(stream, that.data);

    static_assert(noexcept(PushBack(tmp.release())), "Hey, we assume that PushBack is exception safe!!");
    PushBack(tmp.release()); // if no exception, then we could release holder
  }

  ListNode *p = head;
  do
  {
    auto it = pointers.find(p->rand);
    if (it == pointers.end())
      throw general_serialzation_failure();

    p->rand = it->second;
    p = p->next;
  } while (p != head);   // I assume that list is round robin.
}