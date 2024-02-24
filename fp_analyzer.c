 /*
  2  * Author: Grant Zobel - zobelg@bc.edu
  3  */

#include "fp_analyzer.h"

FP_TYPE power_of_2(int exponent) {
    FP_TYPE result = 1.0;
    if(exponent >= 0) {
        for(int i = 0; i < exponent; ++i) {
            result *= 2.0;
        }
    }
    else {
         for(int i = 0; i < (exponent * -1); ++i) {
            result /= 2.0;
        }
    }
    return result;
}

void print_bits(unsigned I_TYPE input, int size) {
    unsigned I_TYPE mask = (I_TYPE)1u << (size - 1);
    while(mask) {
        putchar((!! (input & mask)) + '0');
        mask >>= 1;
    }
    putchar('\n');
}

void print_components(Converter input) {
    printf("%f\n", input.f);
    printf("All bits: ");
    print_bits(input.u, (sizeof(I_TYPE) * 8));
    printf("Sign: ");
    print_bits(input.components.sign, SIGN_SIZE);
    printf("Exponent: ");
    print_bits(input.components.exponent, EXPONENT_SIZE);
    printf("Mantissa: ");
    print_bits(input.components.mantissa, MANTISSA_SIZE);
}

void print_denormalized(Converter input) {
    if(!!input.components.mantissa) {
        int adjust = sizeof(FP_TYPE) == 4? 127 : 1023;
        FP_TYPE mantissa_adjust = input.components.mantissa / power_of_2(MANTISSA_SIZE);
        int negative = 1 - (input.components.sign * 2);
        FP_TYPE net_2_power = power_of_2(adjust - 1);
    
        printf("Original value:\n");
        printf("(-1)^{%d} x %.45lf x 2^{1 - %d}\n", input.components.sign, 
        mantissa_adjust, adjust);
        printf("  = %d x %lf x 2^{%d}\n", negative, mantissa_adjust, (1 - adjust));
        if(negative > 0) {
            printf("  =  %.45lf x ", mantissa_adjust);
        }
        else {
            printf("  = %d x %.45lf x ", negative, mantissa_adjust);
        }
        printf("1/%.0lf\n", net_2_power);
        printf("  = %.45lf\n", input.f);
    }
    else {
        printf("Original value: ");
        input.components.sign > 0 ? printf("%.1lf\n", input.f) : printf("%.1lf\n", input.f);
    }
}

void print_normalized(Converter input) {
    int adjust = sizeof(FP_TYPE) == 4? 127 : 1023;
    FP_TYPE mantissa_adjust = input.components.mantissa / power_of_2(MANTISSA_SIZE);
    int negative = 1 - (input.components.sign * 2);
    int net_exponent =  input.components.exponent - adjust;
    FP_TYPE net_2_power = power_of_2(net_exponent);
    

    printf("Original value:\n");
    printf("(-1)^{%d} x (1 + %lf) x 2^{%d - %d}\n", input.components.sign, 
    mantissa_adjust, input.components.exponent, adjust);
    printf("  = %d x %lf x 2^{%d}\n", negative, (1 + mantissa_adjust), net_exponent);
    if(negative > 0) {
        printf("  =  %lf x ", (1 + mantissa_adjust));
    }
    else {
        printf("  = %d x %lf x ", negative, (1 + mantissa_adjust));
    }
    (net_exponent >= 0)? printf("%.0lf\n", net_2_power) : 
    printf("1/%.0lf\n", power_of_2(net_exponent * -1));
    printf("  = %.45lf\n", input.f);
}

void print_reconstitution(Converter input) {
    if(!input.components.exponent) {
        print_denormalized(input);
    }
    else {
        print_normalized(input);
    }
}

