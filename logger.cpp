#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

// ��������� Group<String> ��� ������������� ���� String
// ��������� vector<String>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����� � vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// ��� ���� ��������� ������� ����
// ��������� ������ ����������� typename String::value_type
// ������������ Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	map<set<Char<String>>, vector<String>> data;
	for (auto& a : strings) {
		set<Char<String>> temp;
		for (auto& i : a)
		{
			temp.insert(i);
		}
		data[move(temp)].push_back(move(a));
	}
	vector<Group<String>> result;
	for (auto& a : data)
		result.push_back(move(a.second));
	return result;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // ������� ����� �� ����� ��������
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // ������� ����� �� ����� ��������
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}
void MyTest() {

  vector<string> strings = {};
  auto groups = GroupHeavyStrings(strings);

  sort(begin(groups), end(groups));  // ������� ����� �� ����� ��������
  ASSERT(groups.empty());

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  RUN_TEST(tr, MyTest);
  return 0;
}
