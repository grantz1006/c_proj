# author: Grant Zobel - zobelg@bc.edu

EC = encode
DC = decode
FLAGS = -Wall -Werror -std=c99
CC = gcc
all: $(EC) $(DC)
$(EC): $(EC).c
	$(CC) $(FLAGS) $(EC).c -o $(EC)
$(DC): $(DC).c
	$(CC) $(FLAGS) $(DC).c -o $(DC)
clean:
	rm -rf $(EC)
	rm -rf $(DC)      
