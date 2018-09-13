#include "test_runner.h"
#include "profile.h"
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;



int main() {

using list_it = list<int>::iterator;
list<int> list_s;



int q;
cin >> q;
vector<list_it> sportsmen_vec(100000, list_s.end());
sportsmen_vec.reserve(100000);
//if (sportsmen_vec[10] == list_s.end())
//cerr << "fasfa";

for (auto i = 0; i < q;++i) {
	int num;
	int pos;
	cin >> num;
	cin >> pos;
	auto it = list_s.insert(sportsmen_vec[pos], num);
	//cout << *it << endl;
	sportsmen_vec[num] = it;

}

for (auto a : list_s)
cout << a << " ";

return 0;
}
