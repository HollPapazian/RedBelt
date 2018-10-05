#include "test_runner.h"

#include <vector>
#include <iterator>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;


// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {

	vector<Sentence<Token>> result;
	//bool last_flag = false;
	auto begin = tokens.begin();
	auto last = tokens.begin();
	for (auto& a : tokens) {
		++last;
		if (last == tokens.end()) {
			result.push_back(Sentence<Token>{make_move_iterator(begin),make_move_iterator(last)});
				} else if (a.IsEndSentencePunctuation() && last->IsEndSentencePunctuation() == false) {
					result.push_back(Sentence<Token>{make_move_iterator(begin),make_move_iterator(last)});
			begin = last;
		}
	}

return result;

}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );
}

void Mytest(){
	ASSERT_EQUAL(
	    SplitIntoSentences(vector<TestToken>({{}})),
	    vector<Sentence<TestToken>>({
	        {{}}
	    })
	  );

	  ASSERT_EQUAL(
	    SplitIntoSentences(vector<TestToken>({{"Split", true}, {"into", true}, {"sentences"}, {"!", true}})),
	    vector<Sentence<TestToken>>({
	        {{"Split", true}, {"into", true}},
			{{"sentences"}, {"!", true}}
	    })
	  );
	  ASSERT_EQUAL(
	    SplitIntoSentences(vector<TestToken>({{"Split", true}, {"into", true}, {"sentences"}, {"!"}})),
	    vector<Sentence<TestToken>>({
	        {{"Split", true}, {"into", true}},
			{{"sentences"}, {"!"}}
	    })
	  );

	  ASSERT_EQUAL(
	    SplitIntoSentences(vector<TestToken>({{"!", true}})),
	    vector<Sentence<TestToken>>({
	        {{"!", true}}
	    })
	  );

	  ASSERT_EQUAL(
	    SplitIntoSentences(vector<TestToken>({{"Split", true}, {"!"}})),
	    vector<Sentence<TestToken>>({
	        {{"Split", true}},
			{{"!"}}
	    })
	  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  RUN_TEST(tr, Mytest);

  return 0;
}
