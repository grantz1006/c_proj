/*
 * Author: Grant Zobel - zobelg@bc.edu
 */

#include "fp_analyzer.h"

FP_TYPE power_of_2(int);

void print_bits(unsigned int, int);

void print_components(Converter);

void print_denormalized(Converter);

void print_normalized(Converter);

void print_reconstitution(Converter);

int main(int argc, char * argv[]) {
    union Converter input;
    if ((argc > 1) && (! strcmp(argv[1], "special"))) {
        FP_TYPE special_values[4] = { 1.0 / 0.0, -1.0 / 0.0, NAN, -NAN };
        for (int i = 0; i < sizeof(special_values) / sizeof(FP_TYPE); ++i) {
            input.f = special_values[i];
            print_components(input);
            putchar('\n');
        }
    }
    else {
        printf("Please enter a floating-point number or q to quit.\n");
        printf("> ");
        while (scanf(FP_SPECIFIER, &input.f)) {
            print_components(input); 
            print_reconstitution(input);
            printf("> ");
        }
    }
    return EXIT_SUCCESS;
}
