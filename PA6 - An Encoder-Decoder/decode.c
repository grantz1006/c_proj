/*
 * Author: Grant Zobel - zobelg@cslab.bc.edu
 */

#include <stdio.h>
#include <stdlib.h>

char decode(const int count, const int bits[]) {
    char original_char;
    for (int i = 0; i < count; ++i) {
        int index = (bits[i] + 8 - count) % 8;
        original_char = (original_char) | (1u << index);
    }
    return original_char;
}

void read_and_decode_file(const char * file_name) {
    FILE * cipher;
    if ((cipher = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "File not found\n");
    }
    else {
        int num_bits = (int)(fgetc(cipher) - '0');
        while (!feof(cipher)) {
            char ascii[num_bits];
            int bits[8] = {0};
            fgets(ascii, num_bits + 1, cipher);
            for (int i = 0; i < num_bits; ++i) {
                bits[i] += (ascii[i] - '0');
            }
            printf("%c", decode(num_bits, bits));
            num_bits = (int)(fgetc(cipher) - '0');
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please enter an input file name as a command-line argument.\n");
        return EXIT_FAILURE;
    }
    else {
        read_and_decode_file(argv[1]);
    }
}
