#include "list.h"
#include <unordered_map>
#include <memory>
#include <iterator>
using namespace std;

/* DISCLAIMER:
 * For demonstration purposes i assumed that stream is NOT binary
 */

namespace
{
  static const string magic_word = "LIST";
  static const int serialize_version = 0;

  static_assert(sizeof(int) == sizeof(ListNode *), "Cannot perform List::Serialization on this machine");
};

void List::Serialize(ostream & stream) const
{
  // I hope that we need speed while we saving state
  // because
  // it could be autosave in midgame, while player need PERFORMANCE
  // so
  // i prefered speed over memory at this moment
  // we will pay debt at deserialization

  stream << magic_word << ":" << serialize_version << ' ';
  stream << count;

  if (!head)
    return; // Serialisation finished
  if (!tail)
    throw general_serialzation_failure();

  auto *current = head;
  auto counter = 0;

  stream << "[";

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
      stream << reinterpret_cast<int>(&that) << ' ';
      stream << reinterpret_cast<int>(that.rand) << ' ';

      stream << that.data.size() << ' '; // that faster
      stream << that.data; // ASCII ONLY!!
      //stream << '|';
    }
    
    current = current->next; // Instead of current++
  } while (current != nullptr);

  stream << "]";
}

namespace
{
  void DataDeserialize(istream & stream, string & str)
  {
    int data_size;
    stream >> data_size;
    stream.ignore(1); // since we in text mode :(
    if (data_size < 0)
      throw List::general_serialzation_failure();
    str.resize(data_size);

    stream.read(&str[0], data_size);
  }
  ListNode *DeserializePointer(istream & stream)
  {
    int tmp;
    stream >> tmp;
    return reinterpret_cast<ListNode *>(tmp);
  }
}

void List::Deserialize(istream & stream)
{
  if (head || tail || count)
    throw general_serialzation_failure(); // out of task.

  { // Check that we could deserialize this
    std::istream_iterator<char> it = stream;
    bool is_equal = equal(magic_word.begin(), magic_word.end(), it);
    if (!is_equal)
      throw general_serialzation_failure();

    //char woot;
    //copy(istream_iterator<char>(stream), istream_iterator<char>(), ostream_iterator<char>(cout));

    int version;
    stream >> version;
    if (version != serialize_version)
      throw general_serialzation_failure();
  }

  int serialized_count;
  stream >> serialized_count;

  if (!serialized_count)
    return;

  // maps old one into new one
  unordered_map<ListNode *, ListNode *> pointers(serialized_count);

  stream.ignore(1); // [
  // Data Loading, rand pointers invalid
  for (auto i = 0; i < serialized_count; i++)
  {
    unique_ptr<ListNode> tmp = make_unique<ListNode>();
    ListNode &that = *tmp;
    ListNode *old_invalid_pointer = DeserializePointer(stream);
    ListNode *encrypted_rand_pointer = DeserializePointer(stream);
    DataDeserialize(stream, that.data);

    pointers.insert({ old_invalid_pointer, &that });

    static_assert(noexcept(PushBack(tmp.release())), "Hey, we assume that PushBack is exception safe!!");
    PushBack(tmp.release()); // if no exception, then we could release holder
    tail->rand = encrypted_rand_pointer;
  }
  stream.ignore(1); // ]

  ListNode *p = head;
  do
  {
    if (p->rand)
    {
      auto it = pointers.find(p->rand);
      if (it == pointers.end())
        throw general_serialzation_failure();
      p->rand = it->second;
    }

    p = p->next;
  } while (p != nullptr);
}