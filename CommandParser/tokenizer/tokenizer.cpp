#include "tokenizer.h"

namespace parser{
tokenizer::tokenizer(const std::string& input){
	this -> input = input;
	cursor = (this -> input).begin();
	
}

void tokenizer::tokenize_input_stream(){
	while(true){
		auto extracted_token = get_next_token();
		if(extracted_token.first == "END-TOKEN"){
			break;	
		}
		tokens.push_back(extracted_token);
	}
}

void tokenizer::print_tokens(){
	std::cout << "TOKENS: " << '\n';
	for(const auto& [token_type, token_value] : tokens)
		std::cout << "{TOKEN_TYPE:" << token_type << ", TOKEN_VALUE:" << token_value << "}" << '\n';
}

token tokenizer::get_next_token(){
	if(not stream_has_tokens()) return token("END-TOKEN", "END");
	std::string::iterator original_cursor_pos = cursor;
	for(const auto& [token_type, token_regex] : token_types){
		std::sregex_iterator input_stream_begin(cursor, (this -> input).end(), token_regex);
		std::sregex_iterator input_stream_end = std::sregex_iterator();
		if(not std::distance(input_stream_begin, input_stream_end)){
			std::cout << "CANNOT EXTRACT " << token_type << " TOKEN" << std::endl;
			continue;
		}
		std::smatch match = *input_stream_begin;
		cursor += match.str().length();
		return token(token_type, match.str());
	}
	cursor = original_cursor_pos;
	return token("INVALID TOKEN", "INVALID TOKEN");
}

bool tokenizer::stream_has_tokens(){
	return cursor != input.end();
}
}
