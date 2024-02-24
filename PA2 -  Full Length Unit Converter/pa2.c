/*
 *
 *author: Grant Zobel - zobelg@bc.edu  
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CM_PER_IN 2.54 
#define CM_PER_KM 100000
#define CM_PER_M 100
#define IN_PER_FT 12
#define IN_PER_MI 63360
#define IN_PER_YD 36
#define MAX_UNIT_LENGTH 256
#define MM_PER_CM 10 

typedef enum Unit{MM, CM, M, KM, IN, FT, YD, MI, UNSUPPORTED} Unit;
typedef enum Direction{INPUT, OUTPUT} Direction;

const char * unit_strings[8] = { "mm", "cm", "m", "km", "in", "ft", "yd", "mi"};

Unit unit_string_to_unit(const char * const unit_string){
    for(int i = 0; i < 8; ++i){
        if (!strcmp(unit_string, unit_strings[i])){
            return (Unit)i;
        } 
    }
    return UNSUPPORTED;
}

int is_metric(const Unit unit){
    return (int)((int)unit < 4);
}

double convert_metric(const double value, const Unit unit, const Direction direction){
    switch(unit) {

    case MM :
        return direction == INPUT? value / MM_PER_CM : value * MM_PER_CM;

    case CM :
        return direction == INPUT? value : value;
 
    case M :
        return direction == INPUT? value * CM_PER_M : value / CM_PER_M;

    case KM :
        return direction == INPUT? value * CM_PER_KM : value / CM_PER_KM;

    default:
        
        exit(EXIT_FAILURE); 
    }
}

double convert_us(const double value, const Unit unit, const Direction direction){
    switch(unit) { 
 
    case IN :
        return direction == INPUT? value : value;
    
    case FT :
        return direction == INPUT? value * IN_PER_FT : value / IN_PER_FT;
    
    case YD :
        return direction == INPUT? value * IN_PER_YD : value / IN_PER_YD;
    
    case MI :
        return direction == INPUT? value * IN_PER_MI : value / IN_PER_MI;
    
    default:

        exit(EXIT_FAILURE);
    }
}

double convert(const double value, const char * const input_unit_string, const char * const output_unit_string){
    
    Unit input_enum = unit_string_to_unit(input_unit_string);  
    Unit output_enum = unit_string_to_unit(output_unit_string);
    
    double in_or_cm;

    if(input_enum == UNSUPPORTED || output_enum == UNSUPPORTED){
        return NAN;
    }
    if(is_metric(input_enum)){
        in_or_cm = convert_metric(value, input_enum, INPUT);
        if(output_enum > 3){
            in_or_cm = in_or_cm / CM_PER_IN;
            return convert_us(in_or_cm, output_enum, OUTPUT);
        }
        else{
            return convert_metric(in_or_cm, output_enum, OUTPUT);   
        }
    }
    else{
        in_or_cm = convert_us(value, input_enum, INPUT);
        if(output_enum < 4){
            in_or_cm = in_or_cm * CM_PER_IN;
            return convert_metric(in_or_cm, output_enum, OUTPUT);
        }
        else{
            return convert_us(in_or_cm, output_enum, OUTPUT);
        }
    }
}

int main(int argc, char * argv[]){
    double value;
    char input_string[MAX_UNIT_LENGTH];
    char output_string[MAX_UNIT_LENGTH];

    printf("Please enter a length to convert, ");
    printf("in the form <number> <input-unit> <output-unit>.\n");
    printf("Enter any letter to quit.\n");
    printf("> ");

    while(scanf("%lf %s %s", &value, input_string, output_string) != 0){
        double result = convert(value, input_string, output_string);

        if(!isnan(result)){
            printf("%f %s = %f %s\n", value, input_string, result, output_string);
        }
        else{
            printf("Allowable units:");
            for(int i = 0; i < 8; ++i){
                printf(" %s", unit_strings[i]);
            }
            printf(".\n");
        }

        printf("> ");
    }

    return EXIT_SUCCESS;
}
