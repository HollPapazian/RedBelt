#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <vector>
#include <string_view>
#include <functional>


using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other){
	  if (!(other.word_frequences.empty())) {
	  for (auto& [key, value]: other.word_frequences){
		  word_frequences[key] += value;
	  }
  }}
};

Stats ExploreLine(const set<string>& key_words, const vector<string>& lines) {

	vector<string_view> result;

	for (auto& line : lines) {

		//cout << line << endl;
	string_view str = line;
	if (str.back() == '\n')
		str.remove_suffix(1);
	while (true) {
		size_t space = str.find(' ');
		result.push_back(str.substr(0, space));
		if (space == str.npos)
			break;
		else {
			// ищем новое начало.
			str.remove_prefix(space+1);
			while (str[0] == ' ')
				str.remove_prefix(1);
		}
	}
	}


	// проверяем ключи
	Stats result_map;
	for (auto& a : result){
		if (key_words.find(string(a)) != key_words.end())
			++result_map.word_frequences[string(a)];
	}


	return result_map;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  vector<string> lines;
  for (string line; getline(input, line); ) {
	  lines.push_back(line);
  }
  result += ExploreLine(key_words, lines);
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {

	  //return ExploreKeyWordsSingleThread(key_words, input);

	vector<vector<string>> tempor(5);
	int i = -1;
	for (string line; getline(input, line); ) {
		tempor[++i].push_back(line);
		if (i == 4)
			i = -1;
	  }


	vector<future<Stats>> futures;
	for (auto a : tempor) {

		 futures.push_back(async(ExploreLine, ref(key_words), a));
	}

	 Stats result;
	 for (auto& a : futures) {
	//	 cout << a.get().word_frequences << endl;
		result += a.get();
	 }

	 return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
