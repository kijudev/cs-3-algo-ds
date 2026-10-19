/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

// >------------------------------------------------------------------ constants

#define MAX_NUMBER_SEPARATOR (';')
#define MAX_NUMBER_NONE      (-FLT_MAX)

// >----------------------------------------------------------------- algorithms

float max_number(const char* str) {
    if (!str) {
        return MAX_NUMBER_NONE;
    }

    const char* curr = str;
    char* next       = NULL;
    float max        = MAX_NUMBER_NONE;

    while (*curr != '\0') {
        float value = strtof(curr, &next);

        if (next == curr) {
            break;
        }

        if (value > max) {
            max = value;
        }

        if (*next == MAX_NUMBER_SEPARATOR) {
            curr = next + 1;
        } else {
            break;
        }
    }

    return max;
}

// >----------------------------------------------------------------------- main

void test_mixed(void) {
    printf("---- mixed\n");

    const char str[] = "-42.42;12.34;567.8;-9.12";
    printf("max of \"%s\": %g\n", str, (double)max_number(str));
    printf("\n");
}

void test_all_negative(void) {
    printf("---- all negative\n");

    const char str[] = "-42.42;-12.34;-9.12";
    printf("max of \"%s\": %g\n", str, (double)max_number(str));
    printf("\n");
}

void test_empty(void) {
    printf("---- empty\n");

    const char str[] = "";
    printf("max of \"%s\": %g\n", str, (double)max_number(str));
    printf("\n");
}

int main(void) {
    test_mixed();
    test_all_negative();
    test_empty();

    return 0;
}
