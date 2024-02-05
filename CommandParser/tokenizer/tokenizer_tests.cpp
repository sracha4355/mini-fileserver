#include <iostream>
#include "../../utils.cpp"
#include "tokenizer.h"
using std::cout, std::string;

int main(){
	parser::tokenizer _tokenizer("   get -f -h /file/path/to/file /file/path/to/file/2 -g -h upload delete list files");
	_tokenizer.tokenize_input_stream();
	_tokenizer.print_tokens();
	return 0;
}
