#include "parser.h"
#include "../../utils.cpp"

using std::string, std::cout;
int main(){
	test();
	parser::parser parser;
	string command = "get -r -a /file/ /hello/system /k";
	parser.set_command(utils::trim(command));
	bool is_succ = parser.parse();
	cout << is_succ << " | pos: " << parser.get_pos() << " | length: " << parser.get_length() << '\n';

	return 0;
}
