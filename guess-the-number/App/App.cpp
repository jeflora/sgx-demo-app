#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "Enclave_u.h"
#include "sgx_urts.h"

#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define MAX_SIZE 1024
#define PORT 5300

#define ENCLAVE_FILENAME "enclave.signed.so"
#define SEALED_DATA_FILENAME "../data/sealed_data.dat"


/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

void print_message(const std::string error) {
    std::cout << "[  App  ] " << error << std::endl;
}

void write_sealed_data_to_file(uint8_t* data_sealed, uint32_t ciph_size) {
    /* Adapted from: https://software.intel.com/en-us/forums/intel-software-guard-extensions-intel-sgx/topic/740798 */ 
    std::vector<uint8_t>sealed(data_sealed, data_sealed + ciph_size);

    std::ofstream output(SEALED_DATA_FILENAME, std::ios::binary);

    std::copy( 
        sealed.begin(), 
        sealed.end(),
        std::ostreambuf_iterator<char>(output));
    output.close();
}

//==== OCALLS Implementation ==
void print_string(const char* str) {
    printf("%s\n", str);
}

void print_num(int num) {
    printf("| NUM: %d |\n", num);
}

void send_data_to_stats_manager(uint8_t* sealed, uint32_t ciph_size) {
    
    struct sockaddr_in address; 
    int connection = 0, n_read; 
    struct sockaddr_in serv_addr;
    char message_to_send[MAX_SIZE];
    char buffer[MAX_SIZE] = {0}; 

    write_sealed_data_to_file(sealed, ciph_size);

    memset(message_to_send, 0, MAX_SIZE);
    sprintf(message_to_send, "%d", ciph_size);

    if ((connection = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        print_message("Socket creation error"); 
        return; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  { 
        print_message("Invalid address/ Address not supported "); 
        return; 
    } 
   
    if (connect(connection, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        print_message("Connection Failed"); 
        return; 
    } 

    send(connection , message_to_send , strlen(message_to_send) , 0); 
    print_message("Message sent"); 
    n_read = read( connection , buffer, MAX_SIZE);
}

//=============================

int main(int argc, char* argv[]) {
    sgx_launch_token_t token = {0};
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);

    if(ret != SGX_SUCCESS) {
        print_message("Enclave could not be created!");
        return -1;
    }

    //=== Start the game
    int return_value;
    sgx_status_t status = start_game(global_eid);
    if (status != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    status = send_number(global_eid, &return_value, 25);
    if (status != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    int array[] = {75, 40, 60, 50};
    status = send_numbers(global_eid, &return_value, array, 4);
    if (status != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    /* Destroy the enclave */
    sgx_destroy_enclave(global_eid);

    print_message("It has been nice to play with you!");

    return 0;
}