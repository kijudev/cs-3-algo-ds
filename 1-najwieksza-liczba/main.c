/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char str[] = "-42.42;12.34;567.8;-9.12";
    char* curr = str;
    char* next = NULL;
    float max  = FLT_MIN;

    while (*curr != '\0') {
        float value = strtof(curr, &next);

        if (value > max) {
            max = value;
        }

        if (*next == ';') {
            curr = next + 1;
        } else {
            break;
        }
    }

    printf("The max value is: %f.\n", (double)max);
    return 0;
}
