#pragma once
#include <iostream>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <string>
#include <regex>

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1
//FONTS USED: NIMBUS MONO PS BOLD


/**
* GRAMMAR: 
*	command -> "get" get-command | "upload" upload-command | "delete" delete-command | "list files"
*
* GET-COMMAND GRAMMAR
*	get-command -> flags path-to-filename-on-serv path-to-store-on-client
*
* UPLOAD-COMMAND GRAMMAR
*	upload-command -> flags path-to-store-on-client path-to-filename-on-serv
*
* DELETE-COMMAND GRAMMAR
*	delete-command -> flags path-to-filename-on-serv
* 
* 	path-to-store-on-client -> #regex for a valid filepath
*   path-to-filename-on-serv -> #regex for a valid filepath
*   flags -> flag flags|flag
*	flag -> "-[a-z]"
*
*
*/

void test();

namespace parser{
using Stream = std::ostringstream;
class parser{
	public:
		parser();
		parser(const std::string&);
		void set_command(const std::string&);
		bool parse(); // returns TRUE if parsed command is valid else FALSE
		
		/* FOR DEBUGGING */
		int get_pos() { return pos;}
		int get_length() { return length;}
		/*****************/
		
	private:
		void eat_whitespace();
		int parse_command();
		/* NON-TERMINALS */
		int match_get_command();
		int match_upload_command();
		int match_delete_command();
		int match_flags();
		/*****************/
		/* TERMINALS */
		int match_flag();
		int match_filepath();
		/************/
		
		
		/* MEMBER VARIABLES */
		int check_for_keyword(const std::string& keyword);		
		std::unordered_set<char> ignore = {' ', '\r', '\v', '\f', '\n'};	
		std::string command;
		int pos = -1;
		int length = 0;
	
		//std::string ptr = "^-[a-z]$";
		//std::regex flag_pattern(ptr);
		/********************/
		
};

}//namespace parser
