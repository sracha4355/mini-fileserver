#include <command/command.h>
#include <gtest/gtest.h>

namespace parser{
TEST(CommandTests, getCommandViewOutput){
	tokenizer tokenizer("get -f -i -l /filepath/file /filepath/file");
	tokenizer.tokenize_input_stream();
	parser parser(tokenizer.get_token_stream());
	command command(parser);
	auto command_type = command.get_command_type();
	auto flags = command.get_flag_tokens();
	auto args = command.get_args_tokens();
	ASSERT_EQ(command_type.first, "GET-COMMAND-IDENTIFIER");
	ASSERT_EQ(command_type.second, "get");
	ASSERT_EQ(flags.size(), 3);
	
	auto iterator = flags.begin();
	ASSERT_EQ((*iterator).second, "-f");
	iterator++;
	ASSERT_EQ((*iterator).second, "-i");
	iterator++;
	ASSERT_EQ((*iterator).second, "-l");
	
	iterator = args.begin();
	ASSERT_EQ((*iterator).second, "/filepath/file");
	ASSERT_EQ((*iterator).first, "FILEPATH");
	iterator++;
	ASSERT_EQ((*iterator).second, "/filepath/file");
	ASSERT_EQ((*iterator).first, "FILEPATH");
}

TEST(CommandTests, unparseableCommandPassed){
	tokenizer tokenizer("list nofiles");
	tokenizer.tokenize_input_stream();
	parser parser(tokenizer.get_token_stream());
	EXPECT_THROW({command command(parser); }, InsufficientTokensOrNotParseable);
}

//	delete-command-args -> folderpath| filename | filepath

TEST(CommandTests, getDeleteCommandOutput){
	tokenizer tokenizer("delete /folder/path/");
	tokenizer.tokenize_input_stream();
	parser parser(tokenizer.get_token_stream());
	command command(parser);
	auto command_type = command.get_command_type();
	auto flags = command.get_flag_tokens();
	auto args = command.get_args_tokens();
	
	ASSERT_EQ(command_type.first, "DELETE-COMMAND-IDENTIFIER");
	ASSERT_EQ(command_type.second, "delete");
	ASSERT_EQ(flags.size(), 0);
	ASSERT_EQ(args.size(), 1);

	auto iterator = args.begin();
	ASSERT_EQ((*iterator).second, "/folder/path/");
	ASSERT_EQ((*iterator).first, "FOLDERPATH");
}

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);	
	return RUN_ALL_TESTS();
}
}// namespace parser
