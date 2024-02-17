#include <iostream>
#include <tokenizer/tokenizer.h>
#include <gtest/gtest.h>
using std::cout, std::string;
int main(int argc, char **argv) {
	/*
	parser::tokenizer _tokenizer("   get -f -h /file/path/to/file /file/path/to/file/2 -g -h upload delete list files");
	_tokenizer.tokenize_input_stream();
	_tokenizer.print_tokens();
	*/
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
