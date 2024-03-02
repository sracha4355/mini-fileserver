#include "parser.h"
/**
* NEW-GRAMMAR: 
* 	note: 
*		1. This grammar is equivalent to the one above, but the terminals are in a tokenized form*
*		2. All caps words are tokens for terminal values 
*		3. Whitespace is IMPORTANT in this command format
*
*	command -> GET-COMMAND-IDENTIFER command-body | 
*			   UPLOAD-COMMAND-IDENTIFER upload-command-body | 
*			   DELETE-COMMAND-IDENTIFER delete-command-body | 
*			   LIST-FILES-IDENTIFIER
*
*    
*				  
*
* GET-COMMAND GRAMMAR
*	get-command -> get-command-body
*
* UPLOAD-COMMAND GRAMMAR
*	upload-command -> upload-command-body
*
* DELETE-COMMAND GRAMMAR
*	delete-command -> delete-command-body
*
*   flags -> FLAG flags|FLAG
*   command-body -> flags file-args |
*				    file-args
*
*   upload-command-body -> flags upload-command-args|upload-command-args
*   get-command-body -> flags get-command-args | get-command-args
*   upload-command-args -> filename|filename folderpath|filename filepath|folderpath folderpath
*   delete-command-body -> flags delete-command-args|delete-command-args
*	delete-command-args -> folderpath| filename | filepath
*	get-command-args -> FILEPATH FILEPATH |
*				 FILENAME FILEPATH | 
*				 FOLDERPATH FOLDERPATH |
*
* 	FILEPATH -> #regex for a valid filepath
*	FLAG -> "-[a-z]"
*/
namespace parser{
	parser::parser(const std::vector<token>& tokens){
		this -> tokens = tokens;
		this -> current_token = 0;
	}
	
	inline int parser::tokens_left(){
		if(current_token < tokens.size())
			return tokens.size() - current_token - 1;
		return -1;
	}
	
	bool parser::parse(){
		if(tokens_left() == -1) return false;
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

	bool parser::match_upload_command_args(){
		std::size_t nested_orig_token = current_token;
		reset_token(nested_orig_token);
		if(match_terminal("FILENAME") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}	
			}
		}
		reset_token(nested_orig_token);
		if(match_terminal("FILENAME") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FILEPATH") == PARSE_SUCCESS){	
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(nested_orig_token);
		if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){	
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(nested_orig_token);
		if(match_terminal("FILENAME") == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(nested_orig_token);
		return PARSE_FAILURE;
	}

	bool parser::match_upload_command_body(){
		std::size_t orig_token = current_token;
		if(match_flags() == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_upload_command_args() == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if(match_upload_command_args() == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	bool parser::match_delete_command_args(){
		std::size_t orig_token = current_token;
		if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		if(match_terminal("FILENAME") == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		if(match_terminal("FILEPATH") == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
	bool parser::match_delete_command_body(){
		std::size_t orig_token = current_token;
		if(match_flags() == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if (match_delete_command_args() == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if (match_delete_command_args() == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}

	bool parser::match_command(){
		eat_whitespace();
		std::size_t orig_token = current_token;
		if( tokens[current_token].first == "GET-COMMAND-IDENTIFIER"){
			advance_token();
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_get_command_body() == PARSE_SUCCESS) return PARSE_SUCCESS;
			}
		}
		
		reset_token(orig_token);
		if(tokens[current_token].first == "UPLOAD-COMMAND-IDENTIFIER"){
			advance_token();
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_upload_command_body() == PARSE_SUCCESS) return PARSE_SUCCESS;
			}
		}
		
		reset_token(orig_token);
		if(tokens[current_token].first == "DELETE-COMMAND-IDENTIFIER"){
			advance_token();
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_delete_command_body() == PARSE_SUCCESS) return PARSE_SUCCESS;
			}
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
	
	bool parser::match_flags(){
		std::size_t orig_token = current_token;
		if(match_terminal("FLAG") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_flags() == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if(match_terminal("FLAG") == PARSE_SUCCESS){
				return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}

	bool parser::match_get_file_args(){
		std::size_t orig_token = current_token;
		if(match_terminal("FILEPATH") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FILEPATH")== PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if(match_terminal("FILENAME")== PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FILEPATH")== PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_terminal("FOLDERPATH") == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
		
	bool parser::match_get_command_body(){
		std::size_t orig_token = current_token;
		if(match_flags() == PARSE_SUCCESS){
			if(match_terminal("WHITESPACE") == PARSE_SUCCESS){
				if(match_get_file_args() == PARSE_SUCCESS){
					eat_whitespace();
					return PARSE_SUCCESS;
				}
			}
		}
		reset_token(orig_token);
		if(match_get_file_args() == PARSE_SUCCESS) {
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
	
	bool parser::match_terminal(const std::string& terminal_token_type){
		if(tokens_left() < 0) return PARSE_FAILURE;
		std::size_t orig_token = current_token;
		if(tokens[current_token].first == terminal_token_type){
			advance_token();
			return PARSE_SUCCESS;
		}
		reset_token(orig_token);
		return PARSE_FAILURE;
	}
	
}//namespace parser

