/*
 * Author: Grant Zobel - zobelg@cslab.bc.edu
 */

#include "matrix_mult.h"

void init_matrix(double * matrix, int dim){
    for (int k = 1; k <= (dim * dim); ++k) {
        matrix[k-1] = k;
    }
}

void multiply_chunk(const double * a, const double * b, double * c, int row_start, int chunk, int dim) {
    for (int i = row_start; i < (row_start + chunk); ++i) {
        for (int j = 0; j < dim; ++j) {
            c[(i * dim) + j] = 0.0;
            for (int k = 0; k < dim; ++k) {
                c[(i * dim) + j] += a[(i * dim) + k] * b[(k * dim) + j];
            }
        }
    }
}

void multiply_serial(const double * a, const double * b, double * c, int dim) {
    multiply_chunk(a, b, c, 0, dim, dim);    
}

void multiply_parallel_processes(const double * a, const double * b, double * c, int dim, int num_workers) {
    int size = dim * dim * sizeof(double);
    int num_procs = num_workers - 1;
    int chunk = dim / num_workers;
    int row_start = 0;
    double * cc = (double *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    for (int i = 0; i < num_procs; ++i) {
        int pid = fork();
        if (pid != 0) {
            multiply_chunk(a, b, cc, row_start, chunk, dim);
            exit(EXIT_SUCCESS);
        }
        row_start += chunk; 
    }
    chunk = dim - row_start;
    multiply_chunk(a, b, cc, row_start, chunk, dim);
    int child_pid;
    int status;
    while ((child_pid = wait(&status)) > 0) {
        if (status != EXIT_SUCCESS) {
            fprintf(stderr, "Child process %d has returned with status %d.\n", child_pid, status);
            exit(EXIT_FAILURE);
        }
    }
    memcpy(c, cc, size);
    munmap(cc, size);
}

void multiply_parallel_threads(const double * a, const double * b, double * c, int dim, int num_workers) {
    int num_threads = num_workers - 1;
    int row_start = 0;
    int chunk = dim / num_workers;
    pthread_t threads[num_threads];
    Args args;
    args.a = a;
    args.b = b;
    args.c = c;
    args.dim = dim;
    args.chunk = chunk;
    Args arg_set[num_workers];
    for (int i = 0; i < num_threads; ++i) {
        arg_set[i] = args;
        arg_set[i].row_start = row_start;
        pthread_create(&threads[i], NULL, task, &arg_set[i]);
        row_start += chunk;
    }
    chunk = dim - arg_set[num_workers - 1].row_start;
    args.row_start = dim - chunk;
    task((void *)&args);
    for (int j = 0; j < num_threads; ++j) {
        pthread_join(threads[j], NULL);
    }
}

void * task(void * args_ptr) {
    Args * args = (Args *)args_ptr;
    multiply_chunk(args->a, args->b, args->c, args->dim, args->row_start, args->chunk);
    return NULL;
}

int verify(double * a, double * b, int dim) {
    for (int i = dim; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (a[(i * dim) + j] != b[(i * dim) + j]) {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

struct timeval time_diff(struct timeval * start, struct timeval * end) {
    struct timeval diff;
    diff.tv_sec = end->tv_sec - start->tv_sec;
    diff.tv_usec = end->tv_usec - start->tv_usec;
    if (diff.tv_usec < 0) {
        --diff.tv_sec;
        diff.tv_usec += 1000000;
    }
    return diff;
}

void print_matrix(double * matrix, int dim) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            printf("%7.0f ", matrix[(i * dim) + j]);
        }
        printf("\n");
    }
}

void print_elapsed_time(struct timeval * start, struct timeval * end, char * legend) {
    struct timeval diff = time_diff(start, end);
    printf("Time elapsed for %s: %d %s and %ld %s\n", legend, (int)diff.tv_sec, (diff.tv_sec != 1)? 
    "seconds" : "second", (long)diff.tv_usec, (diff.tv_usec != 1)? "microseconds" : "microsecond");
}

void print_verification(double * checker, double * checkee, int dim, char * legend) {
    int check = verify(checker, checkee, dim);
    printf("Verification for %s: %s.\n", legend, (check == SUCCESS)? "success" : "failure");
}
