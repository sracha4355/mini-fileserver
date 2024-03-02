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


*   file-args -> FILEPATH FILEPATH |
*				 FILENAME FILEPATH | 
*				 FOLDERPATH FOLDERPATH |
*/

TEST(ParserTests, parseInvalidCommand){
	parser::tokenizer tokenizer("    get   -f    -r  -g  /filename1/filename2    ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), false);
}

TEST(ParserTests, parseListFiles){
	parser::tokenizer tokenizer("    list files ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), false);
}

TEST(ParserTestsGetCommand, parseGetCommandWithArgs_Filename_Filepath){
	parser::tokenizer tokenizer("    get   -f    -r  -g  filename1     /filename2    ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
} 

TEST(ParserTestsGetCommand, parseGetCommandWithArgs_Folderpath_Folderpath){
	parser::tokenizer tokenizer("    get   -f    -r  -g  /filename1/ /filename2/    ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsGetCommand, parseGetCommandWithArgs_Filepath_Filepath){
	parser::tokenizer tokenizer("    get  /filename1/f /filename2/f    ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}



//upload-command-body -> flags filename|flags filename folderpath | flags filename filepath | flags folderpath folderpath


TEST(ParserTestsUploadCommand, parseUploadCommandWithArgs_Filename){
	parser::tokenizer tokenizer("upload   -f    filename   ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsUploadCommand, parseUploadCommandWithArgs_filename_folderpath){
	parser::tokenizer tokenizer("upload   -f  filename  /filename/   ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsUploadCommand, parseUploadCommandWithArgs_filename_filepath){
	parser::tokenizer tokenizer("upload   -f  filename  /filename/filename ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsUploadCommand, parseUploadCommandWithArgs_folderpath_folderpath){
	parser::tokenizer tokenizer("upload   -f -r -g -g /filename/  /filename/filename/ ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsUploadCommand, parseUploadCommandWithNoFlagsButWithArgs_folderpath_folderpath){
	parser::tokenizer tokenizer("upload    /filename/  /filename/filename/ ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsUploadCommand, parseUploadCommandWithNoFlagsButWithArgs_filename_filepath){
	parser::tokenizer tokenizer("upload  filename  /filename/filename ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

//delete-command-body -> flags delete-command-body|delete-command-body
//delete-command-args -> folderpath| filename | filepath
TEST(ParserTestsDeleteCommand, parseDeleteCommandWithArgs_folderpath){
	parser::tokenizer tokenizer("delete -f -g -s -f /filename/filename/ ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}
TEST(ParserTestsDeleteCommand, parseDeleteCommandWithArgs_filepath){
	parser::tokenizer tokenizer("delete -f -g -s -f /filename/file.-name/ ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}
TEST(ParserTestsDeleteCommand, parseDeleteCommandWithArgs_filename){
	parser::tokenizer tokenizer("delete -f -g -s -f .j.cpp ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}

TEST(ParserTestsDeleteCommand, parseDeleteCommandWithNoFlagsButWithArgs_filename){
	parser::tokenizer tokenizer("delete _-j.cpp ");
	tokenizer.tokenize_input_stream();
	parser::parser parser(tokenizer.get_token_stream());
	ASSERT_EQ(parser.parse(), true);
}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);	
	return RUN_ALL_TESTS();
}

