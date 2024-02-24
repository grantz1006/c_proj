/*
 * Author: Grant Zobel - zobelg@bc.edu
 *  
 */
#include <stdio.h>
#include <stdlib.h>

double in_to_cm(int in){
        return (double)in * 2.54;
}
double cm_to_in(int cm){
        return (double)cm / 2.54;
}

double convert(int value, char * unit){
	if (unit[0] == 'c' && unit[1] == 'm' && unit[2] == 0) {
		return cm_to_in(value);
	}
	if (unit[0] == 'i' && unit[1] == 'n' && unit[2] == 0) {
		return in_to_cm(value);
	}
	else{
		printf("The unit must be either \"cm\" or \"in\".\n");
		exit(-1);
	}
}

char * output_unit(char * unit){
	if (unit[0] == 'c' && unit[1] == 'm' && unit[2] == 0) {
		return ("in");
	}
	if (unit[0] == 'i' && unit[1] == 'n' && unit[2] == 0) {
 	   	return ("cm");
	}
	else{
		return "";
	}
}

int main(int argc, char *argv[]){
        if(argc != 3){
                printf("Usage: %s <integer> <unit> \n", argv[0]);
                printf("--where <unit> is either \"cm\" or \"in\".\n");
                return EXIT_FAILURE;
        }
        int value = atoi(argv[1]);
        double res = convert(value, argv[2]);
        printf("%d %s = %f %s\n", value, argv[2], res, output_unit(argv[2]));
        return EXIT_SUCCESS;
}

