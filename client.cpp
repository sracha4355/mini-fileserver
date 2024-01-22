#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <sstream>

//Currently stoi is failing because it is not extracting the filesize from the buffer properly

#define PORT 8080
#define FILE_STORE "./fileStore/"
#define BUFFER_SIZE 1024

void recv_file(int, const std::string&);
void upload_file(int, const std::string&);
void initate_upload_command(int, const std::string&);
void initate_get_command(int, const std::string&);
std::string extract_filename(const char * buffer, const size_t& size);
std::streampos get_filesize(const std::string&, bool);


int main(){
	int client_fd;
	struct sockaddr_in server_addr;
	
	std::string hello = "Hello from client";
    char buffer[1024] = { 0 };
	
	std::cout << "Setting up connection with server..." << std::endl;
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0){
		std::cerr << "Error in getting socket for client" << std::endl;
		abort();
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
		std::cerr << "Invalid address given" << std::endl;
		abort();
	}	
	

	int status = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(status < 0){
		std::cerr << "Error in connecting to the server" << std::endl;
		abort();
	}
	std::cout << "Connection to server established" << std::endl;
	
	send(client_fd, &hello[0], hello.length(), 0);
	buffer[recv(client_fd, buffer, sizeof(buffer) - 1, 0)] = '\0';
	std::cout << "Message from server: " << buffer << std::endl;
	
	while (true){
		std::cout << "test-user> ";
		std::string command;
		std::getline(std::cin, command);
		std::cout << "command: " << command << std::endl;
		if(command[0] == 'g' and command.length() > 2 and command[1] == ' '){
			initate_get_command(client_fd, command);
		} 
		else if(command[0] == 'u' and command.length() > 2 and command[1] == ' '){
			initate_upload_command(client_fd, command);
		}
		
	}
	
	close(client_fd);
	
	return 0;
}

void initate_upload_command(int client_fd, const std::string& command){
	send(
		client_fd,
		&command[0], 
		command.length(),
		0
	);
	std::cout << "sent command" << std::endl;
	std::string filename = extract_filename(&command[0], command.length());
	char buffer[BUFFER_SIZE];
	std::cout << "waiting on res" << std::endl;
	ssize_t bytes_recv = recv(
		client_fd,
		buffer,
		BUFFER_SIZE - 1,
		0
	);
	std::cout << "got res" << std::endl;
	if(buffer[0] == '1'){
		std::cout << "file " << filename << " could not be uploaded" << std::endl;
		return;		
	} else if(buffer[0] == '0'){
		std::cout << "Server is ready for upload " << filename << std::endl;
	}
	
	upload_file(client_fd, filename);	
}

void initate_get_command(int client_fd, const std::string& command){
	send(
		client_fd,
		&command[0],
		command.length(),
		0
	);

	recv_file(
		client_fd,
	 	extract_filename(&command[0], command.length())
	);
	
}


std::string extract_filename(const char * buffer, const size_t& size){
	std::vector<char> data;
	for(size_t index = 2; index < size; index++){
		data.push_back(buffer[index]);	
	}
	return std::string(data.data(), size - 2);
}

void recv_file(int client_fd, const std::string& filename){
	// Create output stream for file and get expected filesize
	char buffer[BUFFER_SIZE];
	ssize_t bytes_recv = recv(
		client_fd,
		buffer,
		BUFFER_SIZE - 1,
		0
	);
	
	buffer[bytes_recv] = '\0';
	if(buffer[0] == '1'){
		std::cout << filename << " could not be found on the server" << std::endl;
		return;
	}	
	
	std::ofstream output_file(FILE_STORE + filename, std::ios::binary | std::ios::out);
	size_t index = 2;
	std::ostringstream oss;
	while(buffer[index] != '|'){
		oss << buffer[index];
		index++;
	}	
	
	int filesize = std::stoi(oss.str());
	index++;
	int read_bytes = 0;
	while(true){
		output_file.write(&buffer[index], 1);
		read_bytes++;
		index++;
		if(read_bytes == filesize) break;
		if(index == bytes_recv) {
			index = 0;
			bytes_recv = recv(
				client_fd, 
				buffer,
				BUFFER_SIZE - 1,
				0
			);
			buffer[bytes_recv] = '\0';
		}
	}
	
	output_file.close();
	std::cout << "downloaded file" << std::endl;
}

std::streampos get_filesize(const std::string& filename, bool binary_mode){
	std::ios::openmode flags = std::ios::in | std::ios::ate;
	if(binary_mode)
		flags |= std::ios::binary;
	std::ifstream file(FILE_STORE + filename, flags);
	if(!file.is_open())
		return -1;
	std::streampos pos = file.tellg();
	file.close();
	return pos;
}

void upload_file(int client_fd, const std::string& filename){
	std::streampos filesize = get_filesize(filename, true);
	std::ifstream input_file(FILE_STORE + filename, std::ios::binary | std::ios::in);

	if(filesize == -1 or !input_file.is_open()){
		std::string err_msg = "1";
		send(
			client_fd,
			&err_msg[0],
			1,0
		);
		std::cout << "Error in locating or reading file: " << filename << std::endl;
		return;
	}
	std::ostringstream oss;
	oss << "0|" << std::to_string(filesize) << "|";
	char byte;
	while(input_file.read(&byte, sizeof(char))){
		oss << byte;
	}
	std::string buffer = oss.str();
	send(
		client_fd,
		&buffer[0],
		buffer.length(),
		0
	);

	std::cout << "File uploaded successfully to server" << std::endl;
}




