#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include "utils.cpp"


#define PORT 8080
#define FILE_STORE "./file/"


std::string extract_filename(char * buffer, const size_t& size);
void sendfile(const std::string& filename, int);
void recv_file_command(int, const std::string&);
void recv_file(const std::string& filename, int);
void delete_file(int, const std::string&);
void list_files(int);
std::streampos get_filesize(const std::string& filename, bool);


int main(){
	std::cout << "Setting up server..." << std::endl << "Files will be sent to " << FILE_STORE << std::endl;
	int server_fd, client_fd, opt = 1;
	struct sockaddr_in addr;
	
	char buffer[1024] = { 0 };
    std::string hello = "Hello from server";
    
	// create server socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0){
		std::cerr << "Server socket init failed" << std::endl;
		abort();
	}
	
	int result = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if(result){
		std::cerr << "Error in setting socket options" << std::endl;
		abort();
	}
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int bind_result = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	if(bind_result < 0){
		std::cerr << "Bind failed" << std::endl;
		abort();
	}
	
	std::cout << "Listening for connections.." <<std::endl;
	if(listen(server_fd, 1) < 0){
		std::cerr << "Listen Failed" << std::endl;
		abort();
	}
	
	//int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	socklen_t len = sizeof(addr);
	client_fd = accept(server_fd, (struct sockaddr* ) &addr, &len);
	if(client_fd < 0){
		std::cerr << "Creating fd for client failed" << std::endl;
		abort();
	}
	
	std::cout << "Client connection accepted | with file descriptor value: " << client_fd << std::endl;
	send(client_fd, &hello[0], hello.length(), 0);
	
	ssize_t bytes_recv = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	buffer[bytes_recv] = '\0';

	std::cout << "Message from client: " << buffer << std::endl;
	
	char command_buffer[1024];
	while(true){
		std::cout << "Waiting on command from client ..." << std::endl;
		bytes_recv = recv(
			client_fd,
			command_buffer,
			sizeof(command_buffer) - 1,
			0
		);
		if(bytes_recv <= 0){
			std::cout << "client closed connection" << std::endl;
			break;
		}
		command_buffer[bytes_recv] = '\0';
		
		std::cout << "Command from client: " << command_buffer << std::endl;
		if(command_buffer[0] == 'g' && bytes_recv > 2 && command_buffer[1] == ' '){
			std::string filename = extract_filename(command_buffer, bytes_recv);
			sendfile(filename, client_fd);
		} else if(command_buffer[0] == 'u' && bytes_recv > 2 && command_buffer[1] == ' '){
			recv_file_command(client_fd, extract_filename(command_buffer, bytes_recv));
		} else if(command_buffer[0] == 'd' && bytes_recv > 2 && command_buffer[1] == ' '){
			delete_file(client_fd, extract_filename(command_buffer, bytes_recv));
		} else if(strcmp(command_buffer, "list files") == 0){
			std::cout << "need to send back a list of commands to client" << std::endl;
			list_files(client_fd);
		} else if(strcmp(command_buffer, "exit") == 0){
			break;
		}
		
		
	}
	
	close(server_fd);
	close(client_fd);
	
	return 0;
}
void list_files(int client_fd){
	auto files = utils::get_files_in_directory(FILE_STORE);
	std::ostringstream oss;
	for(const auto& file: files){
		oss << std::to_string(file.filename.length()) 
			<< ":"
			<< std::to_string(file.filesize)
			<< ":"
			<< file.filename
			<< ":";
	}

	std::ostringstream final_oss;
	auto stream_content = oss.str();
	if(stream_content.length() == 0){
		std::cout << "no files to send" << std::endl;
		std::string err_code = "1";
		send(
			client_fd,
			&err_code[0],
			1,
			0
		);
		return;
	}
	oss.str("");
	oss.clear();
	oss << std::to_string(stream_content.length()) << "|" << stream_content;
	auto buffer = oss.str();
	if(buffer.length() == 0){
		std::cout << "no files to send" << std::endl;
		std::string err_code = "1";
		send(
			client_fd,
			&err_code[0],
			1,
			0
		);
		return;
	}
	std::cout << buffer << std::endl;
	send(
		client_fd,
		&buffer[0],
		buffer.length(),
		0
	);

	std::cout << "sent list of files to client" << std::endl;

}

void delete_file(int client_fd, const std::string& filename){
	int del_result = 0; // can be 0 | 1 | 2
	std::string filepath = FILE_STORE + filename;
	ssize_t existence_result = access(filepath.data(), F_OK);
	
	auto send_payload = [client_fd](const char* code){
		send(
			client_fd, 
			code,
			1,
			0
		);
	};
	if(existence_result == -1) {
		std::cout << filename << " does not exists" << std::endl;
		char code[] = "2";
		send_payload(code);
		return;
	}
	std::cout << filename << " exists" << std::endl;
	del_result = remove(filepath.data());
	if(del_result == 0){
		std::cout << "removed " << filename << std::endl;
		char code[] = "0";
		send_payload(code);
	} else {
		std::cout << "could not remove " << filename << std::endl;
		char code[] = "1";
		send_payload(code);
	}
}

void recv_file_command(int client_fd, const std::string& filename){
	bool suitable = true;
	if(!suitable){
		std::string err_msg = "1";
		send(
			client_fd, 
			&err_msg[0],
			err_msg.length(),
			0
		);
		std::cout << "Filename cannot be uploaded" << std::endl;
		return;
	}
	std::string OK = "0";
	send(
		client_fd,
		&OK[0],
		1,
		0
	);	
	
	recv_file(filename, client_fd);		
}
	

std::string extract_filename(char * buffer, const size_t& size){
	std::vector<char> data;
	for(size_t index = 2; index < size; index++)
		data.push_back(buffer[index]);
	return std::string(data.data(), size - 2);
};

std::streampos get_filesize(const std::string& filename, bool binary_mode){
	std::ios::openmode flags = std::ios::in | std::ios::ate;
	if(binary_mode)
		flags |= std::ios::binary;
	std::ifstream file(FILE_STORE + filename, flags);
	if(!file.is_open())
		return -1;
	std::streampos pos = file.tellg();
	return pos;
}

void sendfile(const std::string& filename, int client_fd){
	std::cout << "Attempting transmission of file: " << filename << std::endl;
	std::ifstream input_file(FILE_STORE+filename, std::ios::binary | std::ios::in);
	if(!input_file.is_open()){
		std::cout << "Error file could not be transmitted" << std::endl;
		std::string err_msg = "1";
		send(
			client_fd,
			&err_msg[0],
			1,
			0
		);
		return;
	}
	
	std::streampos filesize = get_filesize(filename, true);
	std::ostringstream oss;
	oss << "0|" + std::to_string(filesize) +"|";
	char byte;
	while(input_file.read(&byte, sizeof(byte))){
		oss << byte;
	}
	std::string buffer = oss.str();
	send(
		client_fd,
		&buffer[0],
		buffer.length(),
		0
	);
	std::cout << "File transmission completed" << std::endl;
}

void recv_file(const std::string& filename, int client_fd){
	std::cout << "Client["<<client_fd<<"] is sending file: " << filename << std::endl;
	size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_recv = recv(
		client_fd,
		buffer,
		sizeof(buffer) - 1,
		0
	);
	buffer[bytes_recv] = '\0';
	

	if(buffer[0] == '1'){
		std::cout << "Client could not upload file: " << filename << std::endl;
		return ;
	}
	std::ofstream fs(FILE_STORE + filename, std::ios::binary | std::ios::out);
	size_t index = 2;	
	std::ostringstream oss;
	while(buffer[index] != '|'){
		oss << buffer[index];
		index++;
	}

	int filesize = std::stoi(oss.str());
	size_t readbytes = 0;
	index++;
	while(true){
		fs.write(&buffer[index], 1);
		readbytes++;
		index++;
		if(readbytes == filesize) break;
		if(index == bytes_recv){
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
	fs.close();
	std::cout << "File: " << filename << "successfully received" << std::endl;
	
}


