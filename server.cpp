#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <sstream>
#include <fstream>

#define PORT 8080
#define FILE_STORE "./file/"


std::string extract_filename(char * buffer, const size_t& size);
void sendfile(const std::string& filename, int);
void recv_file(const std::string& filename, int);
std::streampos get_filesize(const std::string& filename, bool);

int main(){
	std::cout << "Setting up server..." << std::endl;
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
	
	
	// REASON FOR SEG FAULT IS bytes_recv is getting a crazy large number after first recv of command and that is causing us to access another process's memory
	// making recv_bytes ssize_t fixed it

	char command_buffer[1024];
	while(true){
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
		} /*else if(command_buffer[0] == 'u' && bytes_recv > 2 && command_buffer[1] == ' '){
			recv_file(extract_filename(command_buffer, bytes_recv), client_fd);
		}*/
		
	}
	
	close(server_fd);
	close(client_fd);
	
	return 0;
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
	std::cout << "seg1" << std::endl;
	std::ofstream fs(FILE_STORE + filename, std::ios::binary | std::ios::out);
	size_t index = 2;	
	std::ostringstream oss;
	while(buffer[index] != '|'){
		oss << buffer[index];
		index++;
	}
	std::cout << "seg2" << std::endl;
	int filesize = std::stoi(oss.str());
	size_t readbytes = 0;
	index++;
	std::cout << "br: " << bytes_recv << std::endl;
	while(true){
		//std::cout << "index: " << index << " br: " << bytes_recv << std::endl;
		fs.write(&buffer[index], 1);
		//std::cout << "seg3" << std::endl;
		readbytes++;
		index++;
		if(readbytes == filesize) break;
		if(index == bytes_recv){
			//std::cout << "seg4" << std::endl;
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


