#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

// тест

template <class T>
class ObjectPool {
public:
  T* Allocate() {
		if (free.size() != 0) {
			return Switch();
		} else {
			return *((in_use.insert(new T)).first);
		}
		//cerr << in_use.size() << endl;
		//return *(in_use.begin());
  };
  T* TryAllocate() {
		if (free.size() != 0) {
			return Switch();
		} else {
			return nullptr;
		}
  };

  void Deallocate(T* object) {
	 auto it = in_use.find(object);
	 //cerr << *object << " " << *(*it) << endl;
	 if (it == in_use.end())
		 throw invalid_argument("Invalid argument");
	 else {
	 free.push(*it);
	 in_use.erase(it);
	 }

  };

  void PrintTest (){
	  for (auto a : in_use)
		  cerr << *a << endl;
  }

  ~ObjectPool() {
	 for (auto it : in_use)
		 delete it;
	while (free.size() != 0) {
		delete free.front();
		free.pop();
	}
  };

private:
  set<T*> in_use;
  queue<T*> free;
  T* Switch () {
  auto it = in_use.insert(free.front());
  free.pop();
  return *(it.first);
  };
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
