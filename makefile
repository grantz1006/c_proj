# Author: Grant Zobel - zobelg@bc.edu

APP = fp_analyzer
FLAGS = -Wall -Werror -std=c99
CC = gcc
$(APP): $(APP).c
	$(CC) $(FLAGS) $^ main.c -o $@_f
	$(CC) $(FLAGS) -D DOUBLE $^ main.c -o $@_d
$(APP)_f: $(APP).c
	$(CC) $(FLAGS) $^ main.c -o $@
$(APP)_d: $(APP).c
	$(CC) $(FLAGS) -D DOUBLE $^ main.c -o $@
clean:
	rm -rf  $(APP)_f
	rm -rf  $(APP)_d
clean_f:
	rm -rf $(APP)_f
clean_d:
	rm -rf $(APP)_d
