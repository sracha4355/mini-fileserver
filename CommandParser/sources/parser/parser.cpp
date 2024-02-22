#include "parser.h"
/**
* NEW-GRAMMAR: 
* 	note: 
*		1. This grammar is equivalent to the one above, but the terminals are in a tokenized form*
*		2. All caps words are tokens for terminal values 
*		3. Whitespace is IMPORTANT in this command format
*
*	command -> GET-COMMAND-IDENTIFER command-body | 
*			   UPLOAD-COMMAND-IDENTIFER command-body | 
*			   DELETE-COMMAND-IDENTIFER command-body | 
*			   LIST-FILES-IDENTIFIER
*
*   file-args -> FILEPATH FILEPATH |
*				 FILENAME FILEPATH | 
*				 FILEPATH FILENAME |
*				 FILENAME FILENAME
* GET-COMMAND GRAMMAR
*	get-command -> command-body
*
* UPLOAD-COMMAND GRAMMAR
*	upload-command -> command-body
*
* DELETE-COMMAND GRAMMAR
*	delete-command -> command-body
*
*   flags -> FLAG flags|FLAG
*   command-body -> flags file-args |
*				    file-args
*
*
* 	FILEPATH -> #regex for a valid filepath
*	FLAG -> "-[a-z]"
*/
namespace parser{
	parser::parser(const std::vector<token>& tokens){
		this -> tokens = tokens;
		this -> current_token = 0;
	}
	parser::parser(){}
	
	bool parser::parse(){
		bool res = match_command();
		eat_whitespace();
		if (current_token < tokens.size()) return PARSE_FAILURE;
		return res;
	}
	inline void parser::eat_whitespace(){
		while(current_token < tokens.size() and tokens[current_token].first == "WHITESPACE"){
			advance_token();
		}	
	}

	bool parser::match_command(){
		eat_whitespace();
		std::cout << "in match_command" << '\n';
		std::size_t orig_token = current_token;
		if( tokens[current_token].first == "GET-COMMAND-IDENTIFIER" or 
		   	tokens[current_token].first == "UPLOAD-COMMAND-IDENTIFER" or
		   	tokens[current_token].first == "DELETE-COMMMAND-IDENTIFER"
		){
			std::cout << "id'ed get-ci" << '\n';
			advance_token();
			eat_whitespace();
			if(match_command_body() == PARSE_SUCCESS) return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		if(tokens[current_token].first == "LIST-FILES-IDENTIFIER"){
			advance_token();
			eat_whitespace();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	//flags -> FLAG flags|FLAG
	bool parser::match_flags(){
		// LEFT OFF WORKING HERE
		std::size_t orig_token = current_token;
		if(match_flag() == PARSE_SUCCESS){
			std::cout << "found a flag" << '\n';
			eat_whitespace();
			if(match_flags() == PARSE_SUCCESS){
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		if(match_flag() == PARSE_SUCCESS){
			eat_whitespace();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	bool parser::match_file_args(){
		std::cout << "in match_file_args" << '\n';
		std::size_t orig_token = current_token;
		if(match_filepath() == PARSE_SUCCESS){
			std::cout << "matched a filepath" << '\n';
			eat_whitespace();
			if(match_filepath() == PARSE_SUCCESS){
				std::cout << "matched a filepath" << '\n';
				eat_whitespace();
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		if(match_filename() == PARSE_SUCCESS){
			eat_whitespace();
			if(match_filepath() == PARSE_SUCCESS){
				eat_whitespace();
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		if(match_filepath() == PARSE_SUCCESS){
			eat_whitespace();
			if(match_filename() == PARSE_SUCCESS){
				eat_whitespace();
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		if(match_filename() == PARSE_SUCCESS){
			eat_whitespace();
			if(match_filename() == PARSE_SUCCESS){
				eat_whitespace();
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		
		return PARSE_FAILURE;
	}
	
	bool parser::match_filepath(){
		std::size_t orig_token = current_token;
		if(tokens[current_token].first == "FILEPATH"){
			advance_token();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	bool parser::match_filename(){
		std::size_t orig_token = current_token;
		if(tokens[current_token].first == "FILENAME"){
			advance_token();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	bool parser::match_flag(){
		std::size_t orig_token = current_token;
		if(tokens[current_token].first == "FLAG"){
			advance_token();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	bool parser::match_command_body(){
		std::cout << "in match_command_body" << '\n';
		std::size_t orig_token = current_token;
		if(match_flags() == PARSE_SUCCESS){
			eat_whitespace();
			if(match_file_args() == PARSE_SUCCESS){
				std::cout << "found flags and file_args" << '\n';
				eat_whitespace();
				return PARSE_SUCCESS;
			}
		}
		reset_token(orig_token);
		if(match_file_args() == PARSE_SUCCESS) {
			eat_whitespace();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	inline void parser::advance_token(){
		current_token++;
	}
	inline void parser::reset_token(std::size_t orig_token){
		current_token = orig_token;
	}
}//namespace parser
