#include "Enclave_t.h"
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

int send_number(int number) {

    if (number_to_guess == NONE) {
        start_game();
    }

    if (number == number_to_guess) {
        print_string("[Enclave] Congratulations! Your guess is correct!");
        return 1;
    }
    else if(number < number_to_guess) {
        print_string("[Enclave] Oops! Your number is too low! Try again!");
    }
    else {
        print_string("[Enclave] Oops! Your number is too high! Try again!");
    }

    return 0;
}

int send_numbers(int* numbers, int n_numbers) {
    int iter;

    for(iter = 0; iter < n_numbers; ++iter) {
        if(send_number(numbers[iter]))
            return 1;
    }

    return 0;
}
