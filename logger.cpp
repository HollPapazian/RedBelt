#include "test_runner.h"

#include <vector>

using namespace std;

// ��������� Sentence<Token> ��� ������������� ���� Token
// ��������� vector<Token>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����������� � vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
void MoveOut (typename vector<Token>::iterator begin, typename vector<Token>::iterator last, vector<Sentence<Token>>& result) {
	Sentence<Token> temp;
	move(begin, last, back_inserter(temp));
	result.push_back(move(temp));
}


// ����� Token ����� ����� bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {

	vector<Sentence<Token>> result;
	//bool last_flag = false;
	auto begin = tokens.begin();
	auto last = tokens.begin();
	for (auto& a : tokens) {
		++last;
		if (last == tokens.end()) {
			MoveOut(begin, last, result);
				} else if (a.IsEndSentencePunctuation() && last->IsEndSentencePunctuation() == false) {
			MoveOut(begin, last, result);
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

// ���� �������� ����������� �������� ������ TestToken.
// ��� �������� ���������� ����������� � ������� SplitIntoSentences
// ���������� �������� ��������� ����.
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
