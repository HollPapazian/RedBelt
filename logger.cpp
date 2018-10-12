#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>
#include <string>

using namespace std;

template <typename T>
class PriorityCollection {
public:
	PriorityCollection()=default;
  using Id = pair<T, int>*;/* тип, используемый для идентификаторов */;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object){
  data.push_back(make_pair(move(object), 0));
  data_it.insert(&data.back());
  ++priority_count[0];
  return &data.back();
  }
  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin){
  for (auto i = range_begin; i != range_end; ++i) {
	  data.push_back(make_pair(move(*i), 0));
	  data_it.insert(&data.back());
	  *ids_begin = &data.back();
	  ++ids_begin;
	  ++priority_count[0];
  }

  }
  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
	  return data_it.count(id);
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
	  return id->first;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
	  int temp = ++(id->second);
	  ++priority_count[temp];
	  --priority_count[--temp];
	 // cerr << "Promote" << id->first << " " << id->second << endl;
//	  if (temp > max_priority)
//		  max_priority = temp;
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
	int max_priority = prev(priority_count.end(),1)->first;
	return *find_if(data.rbegin(), data.rend(), [&](const auto& i){return i.second == max_priority;});
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
	  int max_priority = prev(priority_count.end(),1)->first;
	  auto it = --data.end();
	  for (; it != data.begin(); it--)
		  if ((*it).second == max_priority) break;

	  data_it.erase(&*it);
	  auto temp = move(*it);
	  data.erase(it);

	  --priority_count[max_priority];
	  if (priority_count[max_priority] == 0)
		  priority_count.erase(max_priority);
	  return move(temp);
  }

private:
  list<pair<T, int>> data;
  set<pair<T, int>*> data_it;
  //int max_priority = 0;
  map<int,int> priority_count;
  // Приватные поля и методы
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {

  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }

}


void MyTest() {

  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(white_id);

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 1);
  }

  strings.Promote(red_id);
  strings.Promote(red_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, MyTest);
  return 0;
}
