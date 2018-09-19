#include <string>
#include "test_runner.h"
#include <list>
using namespace std;

class Editor {
 public:

  Editor(): cursor(0) {data.reserve(1000000); buffer_cut.reserve(1000000);};
  void Left() {if (cursor > 0) cursor--;};
  void Right(){if (cursor < data.length()) cursor++;};
  void Insert(char token) {data.insert(Ref(),token); cursor++;};
	void Cut(size_t tokens = 1) {
		CutAndCopy(tokens, true);
	}	;
	void Copy(size_t tokens = 1) {
		CutAndCopy(tokens, false);
	}	;
  void Paste() {
	  if (buffer_cut != "") {
	  data.insert(Ref(), buffer_cut.begin(), buffer_cut.end());
	  cursor += buffer_cut.length();
	  }
  };
  string GetTextFromList() const {
	  list<char> list_text;
for (char a : data)
	list_text.push_back(a);
string temp1;
temp1.reserve(list_text.size());
for (char a : list_text)
	temp1.push_back(a);

  };
  string GetText() const {return data;};
  size_t Cursor() const {return cursor;}
  string GetBuffer() const {return buffer_cut;};
 private:
  string::iterator Ref() {return begin(data)+cursor;};
  void CutAndCopy(size_t tokens, bool flag) {
		if (tokens != 0) {
			size_t cut_lenght;
			if (tokens > data.length() - cursor)
				cut_lenght = data.length() - cursor;
			else
				cut_lenght = tokens;
			buffer_cut = data.substr(cursor, cut_lenght);
			if (flag) data.erase(Ref(),Ref() + cut_lenght);
		} else
			buffer_cut = "";
  }
  string data;
  string_view buffer;
  string buffer_cut = "";
  size_t cursor;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void Mytest(){
	Editor editor;
	TypeText(editor, "ab");
	ASSERT_EQUAL(editor.GetText(), "ab");
	ASSERT_EQUAL(editor.Cursor(), 2);
	editor.Left();
	editor.Left();
	editor.Cut(1);
	ASSERT_EQUAL(editor.GetText(), "b");

}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");

    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();

    editor.Left();
    editor.Left();
    editor.Cut(3);
   // cout << "Буфер :  " << editor.GetBuffer() << " Курсор : "<< editor.Cursor() << " Текст : "<< editor.GetText() << endl;
   ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");

  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  RUN_TEST(tr, Mytest);
  return 0;
}
