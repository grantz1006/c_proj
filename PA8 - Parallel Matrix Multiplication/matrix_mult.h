/*
 * Author: Grant Zobel - zobelg@cslab.bc.edu
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIM 1024
#define FAILURE -1
#define MAX_LEGEND_SIZE 256
#define NUM_WORKERS 4
#define SUCCESS 0

typedef struct Args {
    const double * a;
    const double * b; 
    double * c;
    int dim;
    int row_start;
    int chunk; 
} Args;

void init_matrix(double * matrix, int dim);
void multiply_chunk(const double * a, const double * b, double * c, int row_start, int chunk, int dim);
void multiply_serial(const double * a, const double * b, double * c, int dim);
void multiply_parallel_processes(const double * a, const double * b, double * c, int dim, int num_workers);
void multiply_parallel_threads(const double * a, const double * b, double * c, int dim, int num_workers);
void * task(void * args_ptr);
int verify(double * a, double * b, int dim);
struct timeval time_diff(struct timeval * start, struct timeval * end);
void print_matrix(double * matrix, int dim);
void print_elapsed_time(struct timeval * start, struct timeval * end, char * legend);
void print_verification(double * checker, double * checkee, int dim, char * legend);
