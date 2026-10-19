/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// >----------------------------------------------------------------- algorithms

uint64_t gcd_unchecked(uint64_t a, uint64_t b) {
    assert(a && b);

    uint64_t d = 0;

    while (a % 2 == 0 && b % 2 == 0) {
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

    return a << d;
}

uint64_t gcd(uint64_t a, uint64_t b) {
    if (!a) {
        return b;
    }

    if (!b) {
        return a;
    }

    return gcd_unchecked(a, b);
}

// >----------------------------------------------------------------------- main

void test_gcd(void) {
    printf("---- gcd\n");

    printf("gcd(48, 18) = %" PRIu64 "\n", gcd(48, 18));
    printf("gcd(17, 5) = %" PRIu64 "\n", gcd(17, 5));
    printf("gcd(0, 5) = %" PRIu64 "\n", gcd(0, 5));
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    printf("gcd_unchecked(48, 18) = %" PRIu64 "\n", gcd_unchecked(48, 18));
    printf("\n");
}

int main(void) {
    test_gcd();
    test_unchecked();

    return 0;
}
