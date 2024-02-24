/*
 * Author: Grant Zobel - zobelg@bc.edu
 */

#include "mem_alloc.h"

Header * free_list = NULL;

// Debugging Functions
void print_list() {
    if (free_list != NULL) {
        Header * header = free_list;
        printf("%p -> ", header);
        while (header->next != NULL) {
            header = header->next;
            printf("%p ->", header);
        } 
        printf("\n");
    }
    else {
        printf("(Empty list.)\n");
    }
}

size_t get_size(Header * header) {
    return (header->size & (0xfffffffffffffffe));
}

void print_header(Header * header) {
    printf("\tAddr :  %p\n", header);
    printf("\tSize :  %zu\n", get_size(header));
    printf("\tPrevious :  %p\n", header->previous);
    printf("\tNext :  %p\n", header->next);
}

// Helper Functions

int is_allocated(Header * header) {
    return header->size & 1;
}

int is_free(Header * header) {
    return !(header->size & 1);
}

void set_allocated(Header * header) {
    header->size |= 1;
}

void set_free(Header * header) {
    header->size &= (0xfffffffffffffffe);   
}

Header * get_header(void * mem) {
    char * mem_char = (char *)mem;
    mem_char -= sizeof(Header);    
    return (Header *)mem_char;
}

int same_page(Header * h1, Header * h2) {
    return (((long unsigned)h1 >> 12) == ((long unsigned)h2 >> 12))? 1 : 0;
}

// Main Functions

int mem_init() {
    Header * header = (Header *)mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    if ((long)header == -1) {
        return FAILURE;
    }
    else {
        free_list = header;
        free_list->size = PAGE_SIZE - (WORD_SIZE * 3); 
        free_list->previous = NULL;
        free_list->next = NULL;
        return SUCCESS;
    }
}

int mem_extend(Header * last) {
    Header * header = (Header *)mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    if ((long)header == -1) {
        return FAILURE;
    }
    else {
        header->size = PAGE_SIZE - (WORD_SIZE * 3);
        header->previous = last;
        header->next = NULL;
        last->next = header;
        return SUCCESS;
    }

}

void * alloc_help(Header * header, size_t size) {
    size_t original_size = header->size;
    set_allocated(header);
    if (header->size > (size + sizeof(Header))) {
        Header * next_header = (Header *)(((char *)header) + size + sizeof(Header));
        Header * h2 = header->next;
        if (h2 != NULL) {
            h2->previous = next_header;
        }
        next_header->next = header->next;
        next_header->previous = header;
        header->next = next_header;
        header->size = size;
        set_allocated(header);
        next_header->size = original_size - size - sizeof(Header);
    }
    return (void *)(((char *)header) + sizeof(Header));
}

void * mem_alloc(size_t requested_size) {
    size_t alligned_size = ((requested_size + 7) >> 3) << 3;
    if (alligned_size > (PAGE_SIZE - sizeof(Header))) {
        return NULL;
    }
    else {
        if(free_list == NULL) {
            int success = mem_init();
            if(success == FAILURE) {
                return NULL;
            }
        }
        Header * header = free_list;
        while (header->next != NULL) {
            if (is_free(header) && (header->size >= alligned_size)) {
                return alloc_help(header, alligned_size);
            }
            header = header->next;
        }
        if (is_free(header) && (header->size >= alligned_size)) {
            return alloc_help(header, alligned_size);
        }
        int success_end = mem_extend(header);
        if (success_end == FAILURE) {
            return NULL;
        }
        else {
            return alloc_help(header->next, alligned_size);
        }
    }
}

void mem_free_next(Header * header) { 
    header->size += header->next->size + sizeof(Header);
    header->next = header->next->next;
    if (header->next != NULL) {
        if (header->next->next != NULL) {
            header->next->next->previous = header;
        }
    }
}


void mem_free(void * ptr) {
    Header * header = get_header(ptr);
    set_free(header);
    char free_test = 1;
    while (free_test) {
        if (header->previous != NULL) {
            if (is_free(header->previous)) {
                header = header->previous;
            }
            else {
                free_test = 0;
            }
        }
        else {
            free_test = 0;
        }
    }
    free_test = 1;
    while (free_test) {
        if (header->next != NULL) {
            if (is_free(header->next)) {
                mem_free_next(header);
            }
            else {
                free_test = 0;
            }
        }
        else {
            free_test = 0;
        }
    }
    if (header->size == (PAGE_SIZE - sizeof(Header))) {
        if (header->next != NULL) {
            header->next->previous = header->previous;
        }
        if (header->previous != NULL) {
            header->previous->next = header->next;
        }
        if (header == free_list) {
            free_list = header->next;
        }
        munmap(((void *)header), PAGE_SIZE);
    }

}

