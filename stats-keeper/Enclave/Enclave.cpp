#include "Enclave_t.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"

#include <vector>
#include <utility> 

std::vector< std::pair<int,int> > historic;

void store_try(uint8_t* sealed, uint32_t ciph_size) {

    sgx_status_t res;
    int guess, return_value;
    size_t plaintext_len = 2;
    uint8_t* plaintext = (uint8_t*) malloc(plaintext_len * sizeof(uint8_t));

    uint32_t plain_size = plaintext_len;

    print_string("-- Unsealing data. --");

    res = sgx_unseal_data((sgx_sealed_data_t *) sealed, NULL, NULL, plaintext, &plain_size);

    if(res != SGX_SUCCESS) {
        print_string("Error during data unsealing.");
        return;
    }

    print_string("-- Data Unsealed Successfully. --");

    guess = (int) plaintext[0];
    return_value = (int) plaintext[1];

    historic.push_back(std::pair<int,int>(guess, return_value));
}

int get_stats(void) {
    print_string("-- Sending Statistics --");
    return historic.size();
}