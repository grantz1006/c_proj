/*
 * Author: Grant Zobel - zobelg@bc.edu
 */

#include <stdio.h>
#include <stdlib.h>

int encode(unsigned char input, int * indices) {
    unsigned char num_bits = 0;
    int count = 7;
    for (int i = 128; i > 0; i = i / 2) {
        if ((input / i) >= 1) {
            ++num_bits;
            indices[count] += count;
            input = input % i;
        }
        else {
            indices[count] = 10;
        }
        --count;
    }
    for (int j = 0; j < 8; ++j) {
        if (indices[j] < 10) {
            indices[j] = (indices[j] + num_bits) % 8;
        }
    }
    return num_bits;
}

void read_and_encode_file(char * in_file, char * out_file) {
    FILE * text = fopen(in_file, "r");
    FILE * cipher = fopen(out_file, "w");
    if (text  == NULL || cipher == NULL) {
        fprintf(stderr, "file not found.\n");
    }
    else {
        unsigned char ch = (unsigned char)fgetc(text);
        while (! feof(text)) {
            int bit_form[8] = {0};
            int num_bits = encode(ch, bit_form);
            fputc((num_bits + '0'), cipher);
            for (int i = 0; i < 8; ++i) {
                if (bit_form[i] < 10) {
                    fputc((bit_form[i] + '0'), cipher);
                }
            }
            ch = (unsigned char)fgetc(text);
        }
        fclose(text);
        fclose(cipher);
    }
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Please enter input and output file names as command-line arguments.\n");
        return EXIT_FAILURE;
    }
    else {
        read_and_encode_file(argv[1], argv[2]);
    }
    return EXIT_SUCCESS;
}
