/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// >----------------------------------------------------------------------- item

typedef struct {
    int value;
    int priority;
} item_t;

item_t item_make(int value, int priority) {
    return (item_t){.value = value, .priority = priority};
}

item_t item_zero(void) { return (item_t){0}; }

int item_eq(const item_t* a, const item_t* b) {
    return a->value == b->value && a->priority == b->priority;
}

void item_print(const item_t* item) {
    printf("item: { value: %d, priority: %d}", item->value, item->priority);
}

void item_swap_unchecked(item_t* a, item_t* b) {
    item_t temp = *a;
    *a          = *b;
    *b          = temp;
}

// >---------------------------------------------------------------------- queue

#define PQUEUE_OK            (1)
#define PQUEUE_ERR           (0)
#define PQUEUE_BASE_CAPACITY (2)
#define PQUEUE_GROWTH_FACTOR (2)
#define PQUEUE_MAX_SIZE      (SIZE_MAX)

typedef struct {
    size_t size;
    size_t capacity;
    item_t* data;
} pqueue_t;

pqueue_t pqueue_make(void) { return (pqueue_t){0}; }

void pqueue_destroy(pqueue_t* pqueue) {
    if (!pqueue) {
        return;
    }

    free((void*)pqueue);
    *pqueue = (pqueue_t){0};
}

size_t pqueue_size(const pqueue_t* pqueue) { return pqueue->size; }

int pqueue_ensure_capacity(pqueue_t* queue, size_t count) {
    if (!queue) {
        return PQUEUE_ERR;
    }

    if (count <= queue->capacity - queue->size) {
        return PQUEUE_OK;
    }

    size_t max_capacity = PQUEUE_MAX_SIZE / sizeof(item_t);
    if (count > max_capacity - queue->size) {
        return PQUEUE_ERR;
    }

    size_t needed = queue->size + count;
    size_t new_capacity =
        queue->capacity ? queue->capacity : PQUEUE_BASE_CAPACITY;

    while (new_capacity < needed) {
        if (new_capacity > max_capacity / PQUEUE_GROWTH_FACTOR) {
            return PQUEUE_ERR;
        }

        new_capacity *= PQUEUE_GROWTH_FACTOR;
    }

    item_t* new_data =
        realloc((void*)queue->data, new_capacity * sizeof(item_t));

    if (!new_data) {
        return PQUEUE_ERR;
    }

    queue->data     = new_data;
    queue->capacity = new_capacity;
    return PQUEUE_OK;
}

size_t pqueue_parent_index(size_t index) { return index / 2; }
size_t pqueue_left_index(size_t index) { return index * 2; }
size_t pqueue_right_index(size_t index) { return index * 2 + 1; }

// [0, 10, 8, 9, 4, 3, 2, 1]
//     ^   ^  ^
//     1   2  3
//
// [0, 10, 8, 9, 4, 3, 2, 1]
//         ^     ^  ^
//         2     4  5

void queue_enqueue_unchecked(pqueue_t* pqueue, item_t item) {
    pqueue_ensure_capacity(pqueue, pqueue->size + 1)
}

// int queue_enqueue(queue_t* queue, item_t item) {}

// item_t queue_peek_unchecked(const queue_t* queue) {}

// int queue_peek(const queue_t* queue, item_t* out_item) {}

// item_t queue_dequeue_unchecked(queue_t* queue) {}

// int queue_dequeue(queue_t* queue, item_t* out_item) {}

void queue_print(const pqueue_t* queue) {}

// >-----------------------------------------------------------------------
// main

int main(void) { return 0; }
