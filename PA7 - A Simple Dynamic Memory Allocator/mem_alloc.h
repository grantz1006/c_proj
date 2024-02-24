/*
 * Author: Grant Zobel - zobelg@bc.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define FAILURE -1
#define PAGE_SIZE 4096
#define SUCCESS 1
#define WORD_SIZE 8

typedef struct Header {
    size_t size;
    struct Header * next;
    struct Header * previous;
} Header;

extern Header * free_list;

int is_allocated(Header * header);
int is_free(Header * header);
void set_allocated(Header * header);
void set_free(Header * header);
Header * get_header(void * mem);
int same_page(Header * h1, Header * h2);

int mem_init();
int mem_extend();
void * mem_alloc(size_t requested_size);
void mem_free(void * ptr);
