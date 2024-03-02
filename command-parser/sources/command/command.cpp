#include <command.h>
#include <parser.h>
#include <tokenizer.h>
namespace parser{
command::command(parser& potential_command){
	if(not potential_command.parse())
		throw InsufficientTokensOrNotParseable("Please check your command: Invalid command given, command not parseable.");
	const std::vector<token> tokens = potential_command.access_tokens();
	auto it = tokens.begin();	
	command_type = std::make_pair((*it).first,(*it).second);
	for(it = tokens.begin() + 1; it != tokens.end(); ++it){
		if((*it).first == "WHITESPACE") continue;
		if((*it).first == "FLAG"){ 
			flag_tokens.push_back(
				std::make_pair((*it).first,(*it).second)
			);
		} else {
			args_tokens.push_back(
				std::make_pair((*it).first,(*it).second)
			);
		}
	}	
}
const token& command::get_command_type(){
	return command_type;
}
const std::vector<token>& command::get_flag_tokens(){
	return flag_tokens;
}
const std::vector<token>& command::get_args_tokens(){
	return args_tokens;
}
	
}//namespace parser
