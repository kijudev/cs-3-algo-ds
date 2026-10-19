/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// >------------------------------------------------------------------ constants

#define LCM_OK  (1)
#define LCM_ERR (0)

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

uint64_t lcm_unchecked(uint64_t a, uint64_t b) {
    assert(a && b);

    return a / gcd_unchecked(a, b) * b;
}

int lcm(uint64_t a, uint64_t b, uint64_t* out_result) {
    if (!a || !b) {
        *out_result = 0;
        return LCM_OK;
    }

    uint64_t quotient = a / gcd_unchecked(a, b);
    if (quotient > UINT64_MAX / b) {
        *out_result = 0;
        return LCM_ERR;
    }

    *out_result = quotient * b;
    return LCM_OK;
}

// >----------------------------------------------------------------------- main

void test_lcm(void) {
    printf("---- lcm\n");

    uint64_t result = 0;

    lcm(48, 18, &result);
    printf("lcm(48, 18) = %" PRIu64 "\n", result);

    lcm(0, 5, &result);
    printf("lcm(0, 5) = %" PRIu64 "\n", result);
    printf("\n");
}

void test_overflow(void) {
    printf("---- overflow\n");

    uint64_t result = 0;
    int ok          = lcm(UINT64_MAX, UINT64_MAX - 1, &result);

    printf("lcm(UINT64_MAX, UINT64_MAX - 1): %s\n", ok ? "ok" : "err");
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    printf("lcm_unchecked(48, 18) = %" PRIu64 "\n", lcm_unchecked(48, 18));
    printf("\n");
}

int main(void) {
    test_lcm();
    test_overflow();
    test_unchecked();

    return 0;
}
