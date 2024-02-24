#author: Grant Zobel - zobelg@bc.edu

APP = div
FLAGS = -Wall -Werror -std=c99
CC = gcc
$(APP): $(APP)1.s $(APP)2.s
	$(CC) $(FLAGS) $(APP)1.s main.c -o $@1
	$(CC) $(FLAGS) $(APP)2.s main.c -o $@2
clean:
	rm -rf  $(APP)1
	rm -rf  $(APP)2                        
