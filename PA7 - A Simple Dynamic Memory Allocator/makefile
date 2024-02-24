# author: Grant Zobel - zobelg@bc.edu

MA = mem_alloc
FLAGS = -Wall -Werror -std=gnu11
CC = gcc
$(MA): $(MA).c main.c
	$(CC) $(FLAGS) $(MA).c main.c -o $(MA)
clean:
	rm -rf $(MA)
