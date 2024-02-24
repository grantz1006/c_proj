 /*
  2  * Author: Grant Zobel - zobelg@bc.edu
  3  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIGN_SIZE 1
#ifdef DOUBLE
#define EXPONENT_SIZE 11
#define FP_SPECIFIER "%lf"
#define FP_TYPE double
#define I_TYPE long
#define MANTISSA_SIZE 52
#else
#define EXPONENT_SIZE 8
#define FP_SPECIFIER "%f"
#define FP_TYPE float
#define I_TYPE int
#define MANTISSA_SIZE 23
#endif

typedef struct Components {
    unsigned long mantissa : MANTISSA_SIZE;
    unsigned int exponent : EXPONENT_SIZE;
    unsigned int sign : SIGN_SIZE;
} Components;

typedef union Converter {
    FP_TYPE f;
    unsigned I_TYPE u;
    struct Components components;
} Converter;


