#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <typename T>
class SimpleVector {
public:
	SimpleVector (size_t size_) {
		data = new T[size_];
		size = size_;
	}
	T& operator[] (size_t index) {
		return data[index];
	}

	const T* begin() const {return data;}
	const T* end() const {return &(data[size]);}
	T* begin() {return data;}
	T* end() {return &(data[size]);}
	~SimpleVector(){
		delete[] data;
	}
private:
	T* data;
	size_t size;
};

template <typename T>
void Print (const SimpleVector<T>& vect) {
	for (const auto& x : vect)
		cout << x << " ";

}

int main() {

	SimpleVector<int> vectorr(5);
	vectorr[1] = 1;
	*vectorr.begin() = 2;
	Print(vectorr);

}
