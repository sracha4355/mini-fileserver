#pragma once
#include <tokenizer.h>
#include <parser.h>

namespace parser{
class command{
	public:
		command(parser&);
		const token& get_command_type();
		const std::vector<token>& get_flag_tokens();
		const std::vector<token>& get_args_tokens();
		
	private:
		token command_type;
		std::vector<token> flag_tokens;
		std::vector<token> args_tokens;
		
};
}//namespace parser

class InsufficientTokensOrNotParseable : public std::exception {
	public:
		InsufficientTokensOrNotParseable(const std::string& err_msg) {
			this -> err_msg = err_msg;
		}
		const char* what() const noexcept override {
		    return err_msg.c_str();
		}

	private:
		mutable std::string err_msg; // Store the error message to keep it valid
};
