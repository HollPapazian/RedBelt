#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <map>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
//	PriorityCollection(){
//		data_values.reserve(1000000);
//		data_link.reserve(1000000);
//	}
  using Id = int /* тип, используемый для идентификаторов */;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object){
  data_values.push_back(move(object));
  data_link.push_back(data.insert(make_pair(0,data_values.size()-1)).first);
  return data_values.size()-1;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin){
	  for (auto a = range_begin; a != range_end; ++a) {
		  data_values.push_back(move(*a));
		  data_link.push_back(data.insert(make_pair(0,data_values.size()-1)).first);
		  *(ids_begin++) = data_values.size()-1;
	  }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
	  return !(data_link[id] == data.end() || id >= data_link.size());
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
	  return data_values[id];
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
	  int old_pr = (*data_link[id]).first;
	  data.erase(data_link[id]);
	  data_link[id] = (data.insert(make_pair(++old_pr,id)).first);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const{
	  int max_pr = (*data.rbegin()).first;
	  int max_id = (*data.rbegin()).second;
	 // cerr << data_values[max_id] << endl;
	  const T& temp = data_values[max_id];
	  return {temp, max_pr};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax(){
	  int max_pr = (*data.rbegin()).first;
	  int max_id = (*data.rbegin()).second;
	  data.erase(prev(data.end(), 1));
	  //auto temp = move(data_values[max_id]);
	  data_link[max_id] = data.end();
	  return {move(data_values[max_id]), max_pr};
  }

private:
  // Приватные поля и методы
  set<pair<int, int>> data; // приоритет и id
  vector<set<pair<int, int>>::iterator> data_link; // хранилище итераторов пар приоритета и ID
  vector<T> data_values; // Хранилище данных
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
    const auto item = strings.GetMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
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

void TestMy() {
  PriorityCollection<int> strings;
  const auto white_id = strings.Add(1);
  const auto yellow_id = strings.Add(2);
  const auto red_id = strings.Add(3);

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.GetMax();
    ASSERT_EQUAL(item.first, 3);
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, 3);
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, 2);
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, 1);
    ASSERT_EQUAL(item.second, 0);
  }

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, TestMy);
  return 0;
}
