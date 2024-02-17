#pragma once
#include <iostream>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <string>
#include <regex>
#include <tokenizer.h>

#define PARSE_SUCCESS 1
#define PARSE_FAILURE 0
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
*/


/**
* NEW-GRAMMAR: 
* 	note: 
*		1. This grammar is equivalent to the one above, but the terminals are in a tokenized form*
*		2. All caps words are tokens for terminal values 
*		3. Whitespace is IMPORTANT in this command format
*
*	command -> GET-COMMAND-IDENTIFER get-command | 
*			   UPLOAD-COMMAND-IDENTIFER upload-command | 
*			   DELETE-COMMAND-IDENTIFER delete-command | 
*			   LIST-FILES-IDENTIFIER
*
*   file-args -> FILEPATH FILEPATH |
				 FILENAME FILEPATH | 
				 FILEPATH FILENAME |
				 FILENAME FILENAME
* GET-COMMAND GRAMMAR
*	get-command -> flags file-args |
*				   file-args
*
* UPLOAD-COMMAND GRAMMAR
*	upload-command -> flags file-args |
*				      file-args
*
* DELETE-COMMAND GRAMMAR
*	delete-command -> flags file-args |
*				      file-args
*
*   flags -> FLAG flags|FLAG
*
* 	FILEPATH -> #regex for a valid filepath
*	FLAG -> "-[a-z]"
*/

void test();

namespace parser{

using Stream = std::ostringstream;
class parser{
	public:
		parser(const std::vector<token>& tokens);
		parser();
		bool parse();

		
		
	private:
		/* NON-TERMINALS */
		bool match_command();
		bool match_get_command();
		bool match_delete_command();
		bool match_upload_command();
		bool match_list_files_command();
		bool match_command_body();
		bool match_flags();
		bool match_file_args();
		/*****************/
		/* TERMINALS */
		bool match_flag();
		bool match_filepath();
		bool match_filename();
		bool match_whitespace();
		/*************/
		/* HELPER FUNCTIONS */
		inline void advance_token();
		inline void reset_token(std::size_t);
		inline void eat_whitespace();
		/********************/
		/* MEMBER VARIABLES */
		std::vector<token> tokens;
		std::size_t current_token = 0;
		/********************/
		
};
}//namespace parser
