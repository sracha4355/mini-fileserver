#include "tokenizer.h"

namespace parser{
tokenizer::tokenizer(const std::string& input){
	reset(input);
}

void tokenizer::reset(const std::string& input){
	this -> input = input;
	cursor = (this -> input).begin();
	tokens.clear();
	stream_fully_tokenized = false;
}
void tokenizer::tokenize_input_stream(){
	if(stream_fully_tokenized) return;
	int	i = 0;
	while(true){
		auto extracted_token = get_next_token();
		//std::cout << i << ": " << extracted_token.first << std::endl;
		if(extracted_token.first == "END"){
			stream_fully_tokenized = true;
			break;
		}
		if(extracted_token.first == "INVALID TOKEN"){
			stream_fully_tokenized = false;
			break;
		}
		tokens.push_back(extracted_token);
		i++;
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
		//std::cout << "current " << *cursor << std::endl;
		std::sregex_iterator input_stream_begin(cursor, (this -> input).end(), token_regex);
		std::sregex_iterator input_stream_end = std::sregex_iterator();
		if(not std::distance(input_stream_begin, input_stream_end))
			continue;
			
		std::smatch match = *input_stream_begin;
		cursor += match.str().length();
		//std::cout << "adding: " << match.str() << std::endl;
		return token(token_type, match.str());
	}

	std::size_t _token = 0;
	std::string::iterator input_beginning = input.begin();
	while(input_beginning != cursor) {
		++input_beginning;
		_token++;
	}
	throw UnexpectedToken(_token, this -> input);
	std::abort(); // should never reach here
}

bool tokenizer::stream_has_tokens(){
	return cursor != input.end();
}

std::vector<token> tokenizer::get_token_stream(){
	return tokens;
}
}
