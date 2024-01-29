#include "parser.h"

void test(){
	std::cout << "Hello parser" << std::endl;	
}

namespace parser{
	parser::parser(const std::string& _command){
		command = _command;
		length = _command.length();
	}
	
	parser::parser(){
		command = "";
		length = 0;
	}
	
	void parser::set_command(const std::string& new_command){
		command = new_command;
		pos = -1;
		length = new_command.length();
	}
	
	void parser::eat_whitespace(){
		while (pos + 1 < length and ignore.find(command[pos + 1]) != ignore.end()) 
			pos++;
	}
	
	bool parser::parse(){
		eat_whitespace();
		int result = parse_command();
		std::cout << "pos: " << pos << std::endl;
		if(pos == length - 1 && result == PARSE_SUCCESS) return true;
		return false;
	}
	
	int parser::check_for_keyword(const std::string& keyword){
		std::ostringstream stream;
		for(size_t i = 0; i < keyword.length(); i++){
			if(pos + 1 >= length) return PARSE_FAILURE; // need to replace this with an exception of somekind
			stream << command[pos + 1];
			pos++;
		}		
		if(stream.str() == keyword) return PARSE_SUCCESS;
		else return PARSE_FAILURE;
	}
		
	int parser::match_upload_command(){
		std::cout << "in upload command" << '\n';
		eat_whitespace();
		return PARSE_FAILURE;
	}
	
	int parser::match_delete_command(){
		std::cout << "in delete command" << '\n';
		eat_whitespace();
		return PARSE_FAILURE;
	}
	
	/*
	flags -> flag flags|flag
	flag -> "-[a-z]"
	*/
	int parser::match_get_command(){
		std::cout << "in get command " << pos << '\n';
		eat_whitespace();
		int orig_pos = pos;
		if(match_flags() == PARSE_FAILURE){
		//std::cout << "parsing flags failed" << std::endl;
			return PARSE_FAILURE;
		}
		//std::cout << "parsing flags succeeded" << std::endl;
		
		eat_whitespace();
		if(match_filepath() == PARSE_SUCCESS){
			eat_whitespace();
			return match_filepath();
		}
		
		return PARSE_FAILURE;
	}
	
	int parser::match_flags(){
		int orig_pos = pos;
		//std::cout << "in match_flags | pos = " << pos << '\n';
		if(match_flag() == PARSE_SUCCESS){
			if(command[pos + 1] == ' '){
				pos++;
				//std::cout << "matched a flag, moving thru the whitespace" << '\n';
				if(match_flags() == PARSE_SUCCESS){
					return PARSE_SUCCESS;
				}
			}
		}
		pos = orig_pos;
		//std::cout << "last rule failed, resetting pos to " << orig_pos << '\n';
		if(match_flag() == PARSE_SUCCESS){
			return PARSE_SUCCESS;
		}
		return PARSE_FAILURE;
	}
	
	int parser::match_flag(){
		//std::cout << "in match_flag | pos = " << pos << '\n';
		int orig_pos = pos;
		Stream stream;
		while(pos + 1 < length and command[pos + 1] != ' '){
			stream << command[pos + 1];
			pos++;
		}
		std::string flag = stream.str();
		std::regex flag_pattern("^-[a-z]$");
		//std::cout << "flag value: " << flag << '\n';
		if(std::regex_match(flag, flag_pattern))
			return PARSE_SUCCESS;
		pos = orig_pos;
		return PARSE_FAILURE;
	}
	
	int parser::match_filepath(){
		std::regex filepath_pattern("((?:[^/]*/)*)(.*)");
		int orig_pos = pos;
		Stream stream;
		while(pos + 1 < length and command[pos + 1] != ' '){
			stream << command[pos + 1];
			pos++;	
		}
		std::string filepath = stream.str();
		if(std::regex_match(filepath, filepath_pattern))
			return PARSE_SUCCESS;
		pos = orig_pos;
		return PARSE_FAILURE;
	}
	
	int parser::parse_command(){
		std::vector<std::string> keywords = {"get", "upload", "delete", "list files"};
		eat_whitespace();
		int orig_pos = pos;
		if(check_for_keyword("get") == PARSE_SUCCESS) 
			return match_get_command();
		else 
			pos = orig_pos;
		
		
		if(check_for_keyword("upload") == PARSE_SUCCESS)
			return match_upload_command();
		else 
			pos = orig_pos;
		
		
		if(check_for_keyword("delete") == PARSE_SUCCESS)
			return match_delete_command();
		else 
			pos = orig_pos;
		
		
		if(check_for_keyword("list files") == PARSE_SUCCESS)
			return PARSE_SUCCESS;
		else 
			pos = orig_pos;
		

		return PARSE_FAILURE;
	}
} //namespace parser

// I'll go with returning the updated pos, will make it easier for debugging
/*
		newPos = check_for_keyword("upload");
		newPos = check_for_keyword("delete");
		newPos = check_for_keyword("list files");
*/
