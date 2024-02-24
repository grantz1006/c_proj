/*
 * Author: Grant Zobel - zobelg@bc.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINS 64
#define BIN_SPAN .05
#define RUNS 50000
#define SAMPLES 10000
#define SCALE 32

double get_mean_of_uniform_random_samples() {
    double sum_random = 0.0;
    for(int i = 0; i < SAMPLES; ++i) {
        sum_random += ((double)rand() / (double)RAND_MAX * 2.0) - 1.0;
    }
    return sum_random / SAMPLES;
}

double populate_values_and_get_mean(double values[]) {
    double average_of_average;
    for(int i = 0; i < RUNS; ++i) {
        double average_value = get_mean_of_uniform_random_samples();
        values[i] = average_value;
        average_of_average += average_value;
    }
    return average_of_average / RUNS;
}

double get_mean_squared_error(double values[], double mean) {
    double mean_squared;
    for(int i = 0; i < RUNS; ++i) {
        mean_squared += (values[i] - mean) * (values[i] - mean);
    }
    return mean_squared / RUNS;
}

void create_histogram(double values[], int counts[]) {
    int j;
    double bin_start;
    double bin_size = BIN_SPAN / (double)BINS;
    for (j = 0, bin_start = -(BIN_SPAN / 2.0); j < BINS; ++j, bin_start += bin_size) {
        for(int k = 0; k < RUNS; ++k) {
            if((values[k] >= bin_start) && (values[k] < (bin_start + bin_size))){
                counts[j] += 1;
            }
        }
    }
}

void print_histogram(int counts[]) {
    int j;
    double bin_start;
    double bin_size = BIN_SPAN / (double)BINS;
    for (j = 0, bin_start = -(BIN_SPAN / 2.0); j < BINS; ++j, bin_start += bin_size) {
        printf("%.4f  ", bin_start + (bin_size / 2.0));
        for(int k = 0; k < (counts[j] / SCALE); ++k) {
            printf("%s", "X");
        }
        printf("\n");
    }
}

int main(int argc, char * argv[]) {
    double * values = (double * )calloc(RUNS, sizeof(double));
    int * counts = (int * )calloc(BINS, sizeof(int));
    double mean = populate_values_and_get_mean(values);
    double variance = get_mean_squared_error(values, mean);
    create_histogram(values, counts);
    print_histogram(counts);
    printf("Sample mean: %f  Sample variance: %f\n", mean, variance);
    return EXIT_SUCCESS;
}
