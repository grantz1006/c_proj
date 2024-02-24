#include <stdio.h>
#include <stdlib.h>

int div_qr(int, int, int *);

int main(int arg, char * argv[]) {
    printf("Please enter an integer dividend and divisor, or \"q\" to quit.\n");
    int dividend;
    int divisor;
    printf("> ");
    while(scanf("%d %d", &dividend, &divisor)) {
        if(divisor == 0) {
            printf("Division by 0 is undefined.\n");
            printf("> ");
            continue;
        }
        else {
            int remainder;
            int quotient = div_qr(dividend, divisor, &remainder);
            printf("%d / %d = %d, r %d\n", dividend, divisor, quotient, remainder);
        }
        printf("> ");
    }
    return EXIT_SUCCESS;
}
