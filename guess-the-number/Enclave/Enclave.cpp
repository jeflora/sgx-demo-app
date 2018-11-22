#include "Enclave_t.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"

#include <string.h>
#include <stdio.h>

#define NONE 0

int number_to_guess = NONE;

void start_game(void) {
    print_string("[Enclave] Generating random number...");
    number_to_guess = 50;
    print_string("[Enclave] Random number generated... Good luck!");
    return;
}

void seal_and_send(int guess, int return_value) {
    sgx_status_t res;
    size_t plaintext_len = 2;
    uint8_t* plaintext = (uint8_t*) malloc(plaintext_len * sizeof(uint8_t));

    plaintext[0] = ( (uint8_t) guess);
    plaintext[1] = ( (uint8_t) return_value);

    uint32_t ciph_size = sgx_calc_sealed_data_size(0, plaintext_len);
    uint8_t* sealed = (uint8_t*) malloc(ciph_size);
    uint32_t plain_size = plaintext_len;

    res = sgx_seal_data(0, NULL, plaintext_len, plaintext, ciph_size, (sgx_sealed_data_t *) sealed);

    if(res == SGX_SUCCESS) {
        print_string("[Enclave] Seal Success!");
    } else {
        print_string("[Enclave] Seal Failed!!!");
    }
   
    send_data_to_stats_manager(sealed, ciph_size);

    return;
}

int send_number(int number) {

    int return_value = 0;

    if (number_to_guess == NONE) {
        start_game();
    }

    if (number == number_to_guess) {
        print_string("[Enclave] Congratulations! Your guess is correct!");
        return_value = 1;
    }
    else if(number < number_to_guess) {
        print_string("[Enclave] Oops! Your number is too low! Try again!");
    }
    else {
        print_string("[Enclave] Oops! Your number is too high! Try again!");
    }

    seal_and_send(number, return_value);

    return return_value;
}

int send_numbers(int* numbers, int n_numbers) {
    int iter;

    for(iter = 0; iter < n_numbers; ++iter) {
        if(send_number(numbers[iter]))
            return 1;
    }

    return 0;
}
