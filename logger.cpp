#include "test_runner.h"
#include <vector>
#include <future>
#include <numeric>
using namespace std;

//template <typename Iterator>
//class IteratorRange {
//public:
//  IteratorRange(Iterator begin, Iterator end)
//    : first(begin)
//    , last(end)
//    , size_(distance(first, last))
//  {
//  }
//
//  Iterator begin() const {
//    return first;
//  }
//
//  Iterator end() const {
//    return last;
//  }
//
//  size_t size() const {
//    return size_;
//  }
//
//private:
//  Iterator first, last;
//  size_t size_;
//};
//
//template <typename Iterator>
//class Paginator {
//private:
//  vector<IteratorRange<Iterator>> pages;
//
//public:
//  Paginator(Iterator begin, Iterator end, size_t page_size) {
//    for (size_t left = distance(begin, end); left > 0; ) {
//      size_t current_page_size = min(page_size, left);
//      Iterator current_page_end = next(begin, current_page_size);
//      pages.push_back({begin, current_page_end});
//
//      left -= current_page_size;
//      begin = current_page_end;
//    }
//  }
//
//  auto begin() const {
//    return pages.begin();
//  }
//
//  auto end() const {
//    return pages.end();
//  }
//
//  size_t size() const {
//    return pages.size();
//  }
//};
//
//template <typename C>
//auto Paginate(C& c, size_t page_size) {
//  return Paginator(begin(c), end(c), page_size);
//}
//
//int64_t CalculateMatrixSumOneTred(const vector<vector<int>>& matrix) {
//	int result = 0;
//	for (auto i = 0; i < (matrix.size() / 2); ++i){
//	result += accumulate(matrix[i].begin(), matrix[i].end(), 0);
//	}
//	return result;
//}


int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {

	future<int> f = async([matrix]{
		int result = 0;
for (size_t i = 0; i < (matrix.size() / 2); ++i){
	result += accumulate(matrix[i].begin(), matrix[i].end(), 0);
}
return result;
	});

	int result = 0;
	for (auto i = (matrix.size() / 2); i < matrix.size(); ++i){
		result += accumulate(matrix[i].begin(), matrix[i].end(), 0);
	}
	return result + f.get();
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
