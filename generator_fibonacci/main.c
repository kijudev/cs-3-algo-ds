/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <stdio.h>
#define J (24)
#define K (55)

#include <stdint.h>

void seed_state(uint64_t* state) {
    for (uint64_t i = 0; i < K; ++i) {
        state[i] = i;
    }
}

uint64_t lfg(uint64_t* state, uint64_t* j, uint64_t* k) {
    uint64_t r = state[*k] + state[*j];
    state[*k]  = r;

    *j = (*j + 1) % J;
    *k = (*k + 1) % K;

    return r;
}

int main(void) {
    uint64_t state[57];
    uint64_t j = J - 1;
    uint64_t k = K - 1;

    seed_state(state);

    uint64_t r1 = lfg(state, &j, &k);
    uint64_t r2 = lfg(state, &j, &k);
    uint64_t r3 = lfg(state, &j, &k);
    uint64_t r4 = lfg(state, &j, &k);

    printf("r1: %lu\n", r1);
    printf("r2: %lu\n", r2);
    printf("r3: %lu\n", r3);
    printf("r4: %lu\n", r4);

    return 0;
}
