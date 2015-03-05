#pragma once

#include <string>
#include <iostream>

struct ListNode {
  ListNode * 	prev = nullptr;
  ListNode * 	next = nullptr;
  ListNode * 	rand = nullptr; // ��������� �� ������������ ������� ������� ������
  std::string 	data;
};


class List {
public:
  void Serialize(std::ostream & stream) const;	// ���������� � ����
  void Deserialize(std::istream & stream);	// �������� �� �����
  struct general_serialzation_failure {};

  List(std::initializer_list<std::string>);
private:
  ListNode * 	head = nullptr;
  ListNode * 	tail = nullptr;
  int 		count = 0;
  void PushBack(ListNode *) noexcept;
};