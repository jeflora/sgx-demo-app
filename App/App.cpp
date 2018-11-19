#include <stdio.h>
#include <iostream>

#include "Enclave_u.h"
#include "sgx_urts.h"


#define ENCLAVE_FILENAME "enclave.signed.so"


/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

//==== OCALLS Implementation ==
void print_string(const char* str) {
    printf("%s\n", str);
}

void print_num(int num) {
    printf("| NUM: %d |\n", num);
}

//=============================

void print_message(const std::string error) {
    std::cout << "[  App  ] " << error << std::endl;
}

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
    if (ret != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    status = send_number(global_eid, &return_value, 25);
    if (ret != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    int array[] = {75, 40, 60, 50};

    status = send_numbers(global_eid, &return_value, array, 4);
    if (ret != SGX_SUCCESS) {
        print_message("ECALL failed");
        return -1;
    }

    /* Destroy the enclave */
    sgx_destroy_enclave(global_eid);

    print_message("It has been nice to play with you!");

    return 0;
}