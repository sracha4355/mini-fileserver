#pragma once
#include <string>
#include <regex>
#include <sstream>
#include <utility>
#include <iostream>

/**
* GET-COMMAND-IDENTIFIER -> get
* UPLOAD-COMMAND-IDENTIFIER -> upload
* DELETE-COMMAND-IDENTIFIER -> delete
* LIST-FILES-COMMAND-IDENTIFIER -> list file
* FILEPATH -> regex for a filepath
* FLAG -> regex for a flag
* WHITESPACE -> " "
*
*/

namespace parser{
using token = std::pair<std::string, std::string>; // TOKEN TYPE, VALUE
using token_spec = std::pair<std::string, std::regex>;
class tokenizer{
	public:
		tokenizer(const std::string&);
		token get_next_token();
		void tokenize_input_stream();
		bool stream_has_tokens(); //check if input string is completely read
		void print_tokens();
		
	private:
		std::vector<token> tokens;
		std::string input;
		std::string::iterator cursor;
		std::vector<token_spec> token_types = {
			token_spec("GET-COMMAND-IDENTIFIER", std::regex("^get")),
			token_spec("UPLOAD-COMMAND-IDENTIFIER", std::regex("^upload")),
			token_spec("DELETE-COMMAND-IDENTIFIER", std::regex("^delete")),
			token_spec("LIST-FILES-COMMAND-IDENTIFIER", std::regex("^list files")),
			//token_spec("FILEPATH", std::regex("^((?:[^/]*/)*)(.*)")),
			token_spec("FLAG", std::regex("^-[a-z]")),
			token_spec("WHITESPACE", std::regex("^ ")),
			token_spec("END", std::regex("^ END"))
		};
		
};
}
