#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
size_t range_size = range_end - range_begin;
if ((range_end - range_begin) < 2)
	return;
vector<typename RandomIt::value_type> temp;
move(range_begin, range_end, back_inserter(temp));
RandomIt range_13 = temp.begin() + range_size / 3;
RandomIt range_23 = temp.begin() + (range_size / 3)*2;
MergeSort(temp.begin(), range_13);
MergeSort(range_13, range_23);
MergeSort(range_23, temp.end());
vector<typename RandomIt::value_type> temp2;
merge(make_move_iterator(temp.begin()), make_move_iterator(range_13), make_move_iterator(range_13), make_move_iterator(range_23), back_inserter(temp2));
merge(make_move_iterator(temp2.begin()), make_move_iterator(temp2.end()), make_move_iterator(range_23), make_move_iterator(temp.end()), range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
