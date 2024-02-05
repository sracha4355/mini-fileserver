#include "tokenizer.h"

namespace parser{
tokenizer::tokenizer(const std::string& input){
	this -> input = input;
	cursor = (this -> input).begin();
	
}

void tokenizer::tokenize_input_stream(){
	if(stream_fully_tokenized) return;
	while(true){
		auto extracted_token = get_next_token();
		if(extracted_token.first == "END"){
			std::cout << "Stream successfully tokenized" << '\n';
			stream_fully_tokenized = true;
			break;
		}
		if(extracted_token.first == "INVALID TOKEN"){
			std::cout << "Invalid token found in input stream" << '\n';
			stream_fully_tokenized = false;
			break;
		}
		tokens.push_back(extracted_token);
	}
}

void tokenizer::print_tokens(){
	std::cout << "TOKENS: " << '\n';
	for(const auto& [token_type, token_value] : tokens){
		std::cout 
			<< "{ \033[37mTOKEN_TYPE: \033[33m" 
			<< token_type 
			<< "\033[0m, \033[37mTOKEN_VALUE: \033[33m\"" 
			<< token_value << "\"\033[0m }" 
			<< '\n';
	}
}

token tokenizer::get_next_token(){
	if(not stream_has_tokens()) return token("END", "END");
	for(const auto& [token_type, token_regex] : token_types){
		std::sregex_iterator input_stream_begin(cursor, (this -> input).end(), token_regex);
		std::sregex_iterator input_stream_end = std::sregex_iterator();
		if(not std::distance(input_stream_begin, input_stream_end))
			continue;
		std::smatch match = *input_stream_begin;
		cursor += match.str().length();
		return token(token_type, match.str());
	}
	return token("INVALID TOKEN", "INVALID TOKEN");
}

bool tokenizer::stream_has_tokens(){
	return cursor != input.end();
}

std::vector<token> tokenizer::get_token_stream(){
	return tokens;
}
}
