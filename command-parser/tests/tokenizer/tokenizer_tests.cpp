#include <iostream>
#include <tokenizer/tokenizer.h>
#include <gtest/gtest.h>
using std::cout, std::string;

namespace parser{

bool token_stream_equality(const std::vector<token>& tokens, const std::vector<token> user_provided_tokens){
	std::size_t index = 0;
	for(const auto& [type, value] : tokens){
		if(user_provided_tokens[index].first != type or user_provided_tokens[index].second != value) 
			return false;
		index++;
	}
	return true;
}

TEST(TokenizerTests, TokenizeGetCommandWithArgs_Filename_Filename_andWhiteSpace){
	parser::tokenizer tokenizer("    get   -f    -r  -g  filename1     filename2    ");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("WHITESPACE", "    "),
		std::make_pair("GET-COMMAND-IDENTIFIER", "get"),
		std::make_pair("WHITESPACE", "   "),
		std::make_pair("FLAG", "-f"),
		std::make_pair("WHITESPACE", "    "),
		std::make_pair("FLAG", "-r"),
		std::make_pair("WHITESPACE", "  "),
		std::make_pair("FLAG", "-g"),
		std::make_pair("WHITESPACE", "  "),
		std::make_pair("FILENAME", "filename1"),
		std::make_pair("WHITESPACE", "     "),
		std::make_pair("FILENAME", "filename2"),
		std::make_pair("WHITESPACE", "    "),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
} 

TEST(TokenizerTests, TokenizeGetCommandWithArgs_Filepath_Filename){
	parser::tokenizer tokenizer("get -f -r -g  filename1 /filename2/f    ");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("GET-COMMAND-IDENTIFIER", "get"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-f"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-r"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-g"),
		std::make_pair("WHITESPACE", "  "),
		std::make_pair("FILENAME", "filename1"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FILEPATH", "/filename2/f"),
		std::make_pair("WHITESPACE", "    "),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
}

TEST(TokenizerTests, TokenizeGetCommandWithArgs_Filepath_Filepath){
	parser::tokenizer tokenizer("get -f -r -g  /filename1/f /filename2/f    ");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("GET-COMMAND-IDENTIFIER", "get"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-f"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-r"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-g"),
		std::make_pair("WHITESPACE", "  "),
		std::make_pair("FILEPATH", "/filename1/f"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FILEPATH", "/filename2/f"),
		std::make_pair("WHITESPACE", "    "),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
}

TEST(TokenizerTests, TokenizeListFile){
	parser::tokenizer tokenizer(" list files ");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("WHITESPACE", " "),
		std::make_pair("LIST-FILES-COMMAND-IDENTIFIER", "list files"),
		std::make_pair("WHITESPACE", " "),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
}

TEST(TokenizerTests, TokenizeUploadCommand){
	parser::tokenizer tokenizer("upload filename/filepath/");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("UPLOAD-COMMAND-IDENTIFIER", "upload"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FILENAME", "filename"),
		std::make_pair("FILEPATH", "/filepath/"),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
}

TEST(TokenizerTests, TokenizeDeleteCommand){
	parser::tokenizer tokenizer("delete -f -g filename/filepath/");
	tokenizer.tokenize_input_stream();
	std::vector<token> _tokens = {
		std::make_pair("DELETE-COMMAND-IDENTIFIER", "delete"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-f"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FLAG", "-g"),
		std::make_pair("WHITESPACE", " "),
		std::make_pair("FILENAME", "filename"),
		std::make_pair("FILEPATH", "/filepath/"),
	};
	ASSERT_EQ(tokenizer.is_stream_tokenized(), true);
	ASSERT_EQ(token_stream_equality(tokenizer.get_token_stream(), _tokens), true);
}

TEST(TokenizerTests, TokenizeInvalidCommandTestShouldThrowException){
	parser::tokenizer tokenizer("delete --f -g filename/filepath/");
	EXPECT_THROW(tokenizer.tokenize_input_stream(), UnexpectedToken);
	ASSERT_EQ(tokenizer.is_stream_tokenized(), false);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
}
