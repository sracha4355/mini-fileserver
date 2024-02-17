#include <parser/parser.h>
#include <tokenizer/tokenizer.h>
#include <gtest/gtest.h>
using std::string, std::cout;

/**
* GET -> look at folders and retrieve -> get /f/g/d/e /f/r/f/
* GET -> look at folders and retrieve -> get /f/g/d/e filename
* GET -> look at folders and retrieve -> get filename filename
* GET -> look at folders and retrieve -> get filename f/g/de/g
* UPLOAD -> upload (filename | filepath) filepath
* DELETE -> delete filename|filepath 
*/

TEST(ParserTests, parseGetCommandWithArgs_Filename_Filename){
	parser::tokenizer tokenizer("    get   -f    -r  -g  filename1     filename2    ");
	tokenizer.tokenize_input_stream();
	//tokenizer.print_tokens();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
} 

TEST(ParserTests, parseInvalidCommand){
	parser::tokenizer tokenizer("    get   -f    -r  -g  filename1/filename2    ");
	tokenizer.tokenize_input_stream();
	//tokenizer.print_tokens();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), false);
}

TEST(ParserTests, parseGetCommandWithArgs_Filepath_Filepath){
	parser::tokenizer tokenizer("    get   -f    -r  -g  /filename1/ /filename2/    ");
	try	{tokenizer.tokenize_input_stream();}
	catch (UnexpectedToken ex) {cout << ex.what() << '\n';}
	tokenizer.print_tokens();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTests, parseGetCommandWithArgs_Filepath_Filename){
	parser::tokenizer tokenizer("    get   -f    -r  -g  /filename1/ filename    ");
	try {tokenizer.tokenize_input_stream();}
	catch (UnexpectedToken ex) {cout << ex.what() << '\n';}
	tokenizer.print_tokens();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTests, parseUploadCommandWithArgs_Filepath_Filename){
	parser::tokenizer tokenizer("    upload   -f    /filename1/file filename    ");
	try {tokenizer.tokenize_input_stream();}
	catch (UnexpectedToken ex) {cout << ex.what() << '\n';}
	tokenizer.print_tokens();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);	
	return RUN_ALL_TESTS();
}

