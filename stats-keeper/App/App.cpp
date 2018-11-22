#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <signal.h>

#include <iostream>
#include <vector>
#include <fstream>


#include "Enclave_u.h"
#include "sgx_urts.h"


#define ENCLAVE_FILENAME "enclave.signed.so"
#define SEALED_DATA_FILENAME "../data/sealed_data.dat"

#define MAX_SIZE 1024
#define PORT 5300 


/* Global EID  */
sgx_enclave_id_t global_eid = 0;

//==== OCALLS Implementation ==
void print_string(const char* str) {
    printf("%s\n", str);
}

//=============================

void print_message(const std::string error) {
    std::cout << "[  App  ] " << error << std::endl;
}

int server_fd;

void cleanup(int sig) {
	close(server_fd);

	/* Destroy the enclave */
    sgx_destroy_enclave(global_eid);

	exit(0);
}

void establish_communication(int);

int main(int argc, char *argv[]) {
    int client_socket, return_value; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	sgx_launch_token_t token = {0};
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);

    if(ret != SGX_SUCCESS) {
        print_message("[Error] Enclave could not be created!");
        return -1;
    }
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		print_message("[Error] Socket failed"); 
		exit(0);
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		print_message("[Error] Setsockopt"); 
		exit(0); 
	} 

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port  
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		print_message("[Error] Bind failed"); 
		exit(0); 
	} 

	if (listen(server_fd, 3) < 0)  { 
		print_message("[Error] Listen"); 
		exit(0); 
	} 

	
	
	while(1) {
		print_message("-- Listening for connections --");
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  { 
			print_message("[Error] Accept"); 
			exit(0); 
		} 

		print_message("-- Accepted a connection --");

		establish_communication(client_socket);
	}
	
	cleanup(0);

    return 0;
}

uint8_t* read_sealed_data(uint32_t ciph_size) {
	/* Adapted from: https://software.intel.com/en-us/forums/intel-software-guard-extensions-intel-sgx/topic/740798 */ 
	
	std::vector<uint8_t> sealed;

	std::ifstream stateFile(SEALED_DATA_FILENAME, std::ios::binary);
	if(stateFile.good()) {
		sealed.clear();
		sealed.assign(
			std::istreambuf_iterator<char>(stateFile), 
			std::istreambuf_iterator<char>());
		stateFile.close();

	} else {
		sealed.resize(ciph_size);
		std::fill(sealed.begin(), sealed.end(), 0);;
	}
	
	uint8_t* data_sealed = (uint8_t*) malloc(ciph_size+1 * sizeof(uint8_t));

	std::copy(sealed.begin(), sealed.end(), data_sealed);
	
	return data_sealed;
}

void establish_communication(int client_socket) {
	int n_read;
	char buffer[MAX_SIZE] = {0};
	char done[] = "Done"; 

	n_read = read( client_socket , buffer, MAX_SIZE); 

	//remove trailing newline
	char *pos;
	if ((pos=strchr(buffer, '\n')) != NULL)
    	*pos = '\0';

	if(!strcmp(buffer, "stats")) {
		int ret;
		sgx_status_t status = get_stats(global_eid, &ret);
		if (status != SGX_SUCCESS) {
			print_message("ECALL failed");
			exit(0);
		}

		char data[MAX_SIZE];
		memset(data, 0, MAX_SIZE);
		sprintf(data, "Number of tries: %d\n", ret);
		send(client_socket , data , strlen(data) , 0 );

		print_message("-- Statistics sent --");
		return;
	}

	uint32_t ciph_size = atoi(buffer);

	uint8_t* sealed = read_sealed_data(ciph_size);

	sgx_status_t status = store_try(global_eid, sealed, ciph_size);
    if (status != SGX_SUCCESS) {
        print_message("ECALL failed");
        return;
    }

	send(client_socket , done , strlen(done) , 0 ); 
	print_message("-- Message sent --");

	close(client_socket);
}
