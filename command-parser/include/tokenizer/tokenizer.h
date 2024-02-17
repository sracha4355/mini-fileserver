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
		bool is_stream_tokenized() { return stream_fully_tokenized;}
		
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
			token_spec("FILENAME", std::regex("^[a-zA-Z0-9_]+([a-zA-Z0-9_.-])*")),
			token_spec("END", std::regex("^ END"))
		};
		bool stream_fully_tokenized = false;

		
};
}

class UnexpectedToken : public std::exception {
public:
    UnexpectedToken(std::size_t current_token, const std::string& _input) : token(current_token), input(_input) {}

    const char* what() const noexcept override {
        std::ostringstream stream;
        std::string header = "Unexpected token at: ";
        stream << header << input << '\n';
        for (std::size_t i = 0; i < header.length() + token; i++) {
            stream << " ";
        }
        stream << "^";
        err_msg = stream.str();
        return err_msg.c_str();
    }

private:
    std::size_t token;
    std::string input;
	mutable std::string err_msg; // Store the error message to keep it valid
};

/*
class UnexpectedToken : public std::exception {
    public:
    	UnexpectedToken(std::size_t current_token, const std::string& _input){
			token = current_token;
    		input = _input;
    	}
		 const char* what() const noexcept override{\
		 	std::ostringstream stream;
			std::string header = "Unexpected token at: ";
			stream << header << input << '\n';
			std::string err_msg = header + input + '\n';
			for(std::size_t i; i < header.length() + token; i++){
				stream << " ";
			}
			stream << "^";
			std::cout << stream.str();
			std::cout << "done";
        	return stream.str().c_str();
	    }
	private:
		std::size_t token;
		std::string input;
};*/


