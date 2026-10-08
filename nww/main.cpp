/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <stdint.h>
#include <stdio.h>

uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t d = 0;

    while (a % 2 == 0 && b && 2 == 0) {
        a /= 2;
        b /= 2;
        ++d;
    }

    while (a % 2 == 0) {
        a /= 2;
    }

    while (b % 2 == 0) {
        b /= 2;
    }

    while (a != b) {
        if (a > b) {
            a -= b;
            while (a % 2 == 0) {
                a /= 2;
            }
        } else {
            b -= a;
            while (b % 2 == 0) {
                b /= 2;
            }
        }
    }

    return (1 << d) * a;
}

int main(void) {
    uint64_t a      = 48;
    uint64_t b      = 18;
    uint64_t result = gcd(a, b);

    printf("result: %lu\n", result);

    return 0;
}
