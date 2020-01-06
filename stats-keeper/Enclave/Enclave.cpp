#include "Enclave_t.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"

#include <vector>
#include <utility> 

std::vector< std::pair<int,int> > historic;

int count_correct_guesses();

void store_try(uint8_t* sealed, uint32_t ciph_size) {

    sgx_status_t res;
    int guess, return_value;
    size_t plaintext_len = 2;
    uint8_t* plaintext = (uint8_t*) malloc(plaintext_len * sizeof(uint8_t));

    uint32_t plain_size = plaintext_len;

    print_string("[Enclave] -- Unsealing data. --");

    res = sgx_unseal_data((sgx_sealed_data_t *) sealed, NULL, NULL, plaintext, &plain_size);

    if(res != SGX_SUCCESS) {
        print_string("Error during data unsealing.");
        return;
    }

    print_string("[Enclave] -- Data Unsealed Successfully. --");

    guess = (int) plaintext[0];
    return_value = (int) plaintext[1];

    historic.push_back(std::pair<int,int>(guess, return_value));
}

int get_stats(void) {
    print_string("[Enclave] -- Sending Statistics --");
    return historic.size();
}

void close_and_store(void) {
    sgx_status_t res;
    size_t plaintext_len = 2;
    uint8_t* plaintext = (uint8_t*) malloc(plaintext_len * sizeof(uint8_t));
    int correct_guesses = count_correct_guesses();
    int incorrect_guesses = historic.size() - correct_guesses;

    plaintext[0] = ( (uint8_t) correct_guesses);
    plaintext[1] = ( (uint8_t) incorrect_guesses);

    uint32_t ciph_size = sgx_calc_sealed_data_size(0, plaintext_len);
    uint8_t* sealed = (uint8_t*) malloc(ciph_size);
    uint32_t plain_size = plaintext_len;

    res = sgx_seal_data(0, NULL, plaintext_len, plaintext, ciph_size, (sgx_sealed_data_t *) sealed);

    if(res == SGX_SUCCESS) {
        print_string("[Enclave] Seal Success!");
    } else {
        print_string("[Enclave] Seal Failed!!!");
    }
   
    write_sealed_data_to_file(sealed, ciph_size);

    return;
}

int count_correct_guesses() {
    int count = 0;
    for(std::vector< std::pair<int,int> >::iterator it = historic.begin(); it != historic.end(); ++it) {
        if(it->second == 1) count++;
    }
    return count;
}
