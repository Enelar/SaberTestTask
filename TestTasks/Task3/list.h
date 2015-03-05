#pragma once

#include <string>
#include <iostream>

struct ListNode {
  ListNode * 	prev = nullptr;
  ListNode * 	next = nullptr;
  ListNode * 	rand = nullptr; // указатель на произвольный элемент данного списка
  std::string 	data;
};


class List {
public:
  void Serialize(std::ostream & stream) const;	// сохранение в файл
  void Deserialize(std::istream & stream);	// загрузка из файла
  struct general_serialzation_failure {};

  List(std::initializer_list<std::string>);
private:
  ListNode * 	head = nullptr;
  ListNode * 	tail = nullptr;
  int 		count = 0;
  void PushBack(ListNode *) noexcept;
};