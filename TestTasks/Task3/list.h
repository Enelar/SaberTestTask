#pragma once

#include <string>
#include <iostream>

struct ListNode {
  ListNode * 	prev = nullptr;
  ListNode * 	next = nullptr;
  ListNode * 	rand = nullptr; // указатель на произвольный элемент данного списка
  std::string 	data;
};

struct ListIterator
{
  ListNode *containee;
  ListIterator &operator++();
  operator ListNode*() const;
};


class List {
public:
  void Serialize(std::ostream & stream) const;	// сохранение в файл
  void Deserialize(std::istream & stream);	// загрузка из файла
  struct general_serialzation_failure {};
  struct fatal_codestyle_issue {};
  struct nullptr_pointer {};

  List(std::initializer_list<std::string>);
  void _DebugSetRandPointers();

  ListIterator begin() const;
  ListIterator end() const;
private:
  ListNode * 	head = nullptr;
  ListNode * 	tail = nullptr;
  int 		count = 0;
  void PushBack(ListNode *) noexcept;

  friend std::ostream  &operator<<(std::ostream  &, const List &);
};

std::ostream &operator<<(std::ostream &, const List &);