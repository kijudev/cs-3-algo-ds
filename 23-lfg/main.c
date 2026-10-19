/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// >------------------------------------------------------------------------ lfg

#define LFG_OK  (1)
#define LFG_ERR (0)
#define LFG_J   (24)
#define LFG_K   (55)

typedef struct {
    uint64_t state[LFG_K];
    size_t j;
    size_t k;
} lfg_t;

void lfg_seed(lfg_t* lfg) {
    if (!lfg) {
        return;
    }

    for (size_t i = 0; i < LFG_K; ++i) {
        lfg->state[i] = i;
    }

    lfg->j = LFG_J - 1;
    lfg->k = LFG_K - 1;
}

lfg_t lfg_make(void) {
    lfg_t lfg = {0};
    lfg_seed(&lfg);

    return lfg;
}

uint64_t lfg_next_unchecked(lfg_t* lfg) {
    assert(lfg);

    uint64_t r         = lfg->state[lfg->k] + lfg->state[lfg->j];
    lfg->state[lfg->k] = r;

    lfg->j = (lfg->j + 1) % LFG_K;
    lfg->k = (lfg->k + 1) % LFG_K;

    return r;
}

int lfg_next(lfg_t* lfg, uint64_t* out_value) {
    if (!lfg) {
        *out_value = 0;
        return LFG_ERR;
    }

    *out_value = lfg_next_unchecked(lfg);
    return LFG_OK;
}

// >----------------------------------------------------------------------- main

void test_next(void) {
    printf("---- next\n");

    lfg_t lfg = lfg_make();

    for (size_t i = 0; i < 4; ++i) {
        uint64_t r = 0;

        if (lfg_next(&lfg, &r)) {
            printf("r%zu: %" PRIu64 "\n", i + 1, r);
        }
    }
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    lfg_t lfg = lfg_make();

    for (size_t i = 0; i < 4; ++i) {
        printf("r%zu: %" PRIu64 "\n", i + 1, lfg_next_unchecked(&lfg));
    }
    printf("\n");
}

int main(void) {
    test_next();
    test_unchecked();

    return 0;
}
