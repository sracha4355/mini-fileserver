#include <parser/parser.h>
#include <tokenizer/tokenizer.h>
#include <gtest/gtest.h>
using std::string, std::cout;
void tokenizer_test(){
	/*
	parser::parser parser;
	parser::tokenizer _tokenizer("get -f -h file/path/to/file file/path/to/file/2 -g -h upload delete list files");
	_tokenizer.tokenize_input_stream();
	_tokenizer.print_tokens();
	string command = "get -r -a /file/ /hello/system /k";
	parser.set_command(utils::trim(command));
	bool is_succ = parser.parse();
	//cout << is_succ << " | pos: " << parser.get_pos() << " | length: " << parser.get_length() << '\n';	
	*/
}
void parser_test(){
	cout << "parser test" << '\n';
	parser::tokenizer _tokenizer("get -f -h /file/path/to/file /file/path/to/file/2");
	_tokenizer.tokenize_input_stream();
	_tokenizer.print_tokens();
	cout << "Here_1" << '\n';
	parser::parser parser(_tokenizer.get_token_stream());
	cout << "Here_2" << '\n';
	bool is_succ = parser.parse();
	cout << is_succ << '\n';		
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);	
	return RUN_ALL_TESTS();
}

