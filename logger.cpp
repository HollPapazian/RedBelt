#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
	  vector<typename RandomIt::value_type> pool;
	  int size = last - first - 1;
	  const size_t size_const = size+1;
	  pool.reserve(size);
	  move(first, last, std::back_inserter(pool));
	  vector<bool> flags(size, true);
	  flags[0] = false;
	  size_t cur_pos = 0;
	  size_t step_counter = 0;
	  *(first++) = move(pool[cur_pos]);

	  while (size>0) {
		  if (flags[cur_pos] == true)
			  step_counter++;
		  if (step_counter == step_size) {
			  step_counter = 0;
			  *(first++) = move(pool[cur_pos]);
			  flags[cur_pos] = false;
			  --size;
		  }
		  ++cur_pos;
		  if (cur_pos == size_const) {
		  			  cur_pos = 0; }
	  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

// ��� ����������� ���, ������� ������� ��� ���������, ��� ���� ����������
// ������� MakeJosephusPermutation �� ��������� ����������� ��������.
// ������ ��, ��������, �� ��������� ��� �� �������, ������ �� ���������,
// ������ �� ������� ������ ���, ����� � ����� ��� move-��������� �
// � ����� ������������� �����

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

void SpeedTest() {

	vector<int> numbers(100000);
	iota(numbers.begin(), numbers.end(), 0);
	{
		LOG_DURATION("Speed")
		MakeJosephusPermutation(begin(numbers), end(numbers), 2);
	}

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  SpeedTest();


  return 0;
}
