#pragma once
#include <string>
#include <regex>
#include <sstream>
#include <utility>
#include <iostream>

namespace parser{
using token = std::pair<std::string, std::string>; // TOKEN TYPE, VALUE
using token_spec = std::pair<std::string, std::regex>;
class tokenizer{
	public:
		tokenizer();
		tokenizer(const std::string&);
		token get_next_token();
		void tokenize_input_stream();
		bool stream_has_tokens(); //check if input string is completely read
		void print_tokens();
		std::vector<token> get_token_stream();
		
	private:
		std::vector<token> tokens;
		std::string input;
		std::string::iterator cursor;
		std::vector<token_spec> token_types = {
			token_spec("GET-COMMAND-IDENTIFIER", std::regex("^get")),
			token_spec("UPLOAD-COMMAND-IDENTIFIER", std::regex("^upload")),
			token_spec("DELETE-COMMAND-IDENTIFIER", std::regex("^delete")),
			token_spec("LIST-FILES-COMMAND-IDENTIFIER", std::regex("^list files")),
			token_spec("FILEPATH", std::regex("^/(?:[a-zA-Z0-9_.-]+/?)+")),
			token_spec("FLAG", std::regex("^-[a-z]")),
			token_spec("WHITESPACE", std::regex("^\\s+")),
			token_spec("FILENAME", std::regex("^[a-zA-Z0-9_.-]+")),
			token_spec("END", std::regex("^ END"))
		};
		bool stream_fully_tokenized = false;
		
};
}

class UnexpectedToken : public std::exception {
    public:
    	UnexpectedToken(std::size_t current_token){
    		err_msg = "Unexpected token at token number: " + std::to_string(static_cast<int>(current_token));
    	}
		char * what () {
        	return err_msg.data();
	    }
	private:
		std::string err_msg;
};


