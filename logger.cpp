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
	  size_t size = last - first;
	  pool.reserve(size);
	  move(first, last, std::back_inserter(pool));
	  size_t cur_pos = 0;
	  //cerr << "Вектор создали" << endl;

	  while (!pool.empty()) {
		//cerr << "Итерация " << i1++ << " cur_pos " << cur_pos << endl;
	    *(first++) = move(pool[cur_pos]);
	    pool.erase(pool.begin() + cur_pos);
	    if (pool.empty()) break;
	    cur_pos = (cur_pos + step_size - 1) % pool.size();
	    //cerr << cur_pos << endl;
	  }




}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
//  {
//    vector<int> numbers_copy = numbers;
//    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
//    ASSERT_EQUAL(numbers_copy, numbers);
//  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

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
