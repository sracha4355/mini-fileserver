#include <parser.h>
#include <tokenizer.h>
int main(){
	parser::tokenizer tokenizer("get");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	return 0;
}
