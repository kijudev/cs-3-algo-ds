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
} item_t;

item_t item_make(int value) { return (item_t){value}; }
item_t item_zero(void) { return (item_t){0}; }

int item_eq(const item_t* a, const item_t* b) { return a->value == b->value; }

void item_print(const item_t* item) { printf("%d", item->value); }

// >---------------------------------------------------------------------- queue

// capacity has to stay a power of two, indices are wrapped with a mask
#define QUEUE_OK            (1)
#define QUEUE_ERR           (0)
#define QUEUE_BASE_CAPACITY (2)
#define QUEUE_GROWTH_FACTOR (2)
#define QUEUE_MAX_SIZE      (SIZE_MAX)

typedef struct {
    size_t tail;
    size_t size;
    size_t capacity;
    item_t* data;
} queue_t;

queue_t queue_make(void) { return (queue_t){0}; }

void queue_destroy(queue_t* queue) {
    if (!queue) {
        return;
    }

    free((void*)queue->data);
    *queue = (queue_t){0};
}

int queue_is_full(const queue_t* queue) {
    if (!queue) {
        return 0;
    }

    return queue->size == queue->capacity;
}

int queue_is_empty(const queue_t* queue) {
    if (!queue) {
        return 1;
    }

    return queue->size == 0;
}

size_t queue_size(const queue_t* queue) {
    if (!queue) {
        return 0;
    }

    return queue->size;
}

int queue_ensure_capacity(queue_t* queue, size_t count) {
    if (!queue) {
        return QUEUE_ERR;
    }

    if (count <= queue->capacity - queue->size) {
        return QUEUE_OK;
    }

    size_t max_capacity = QUEUE_MAX_SIZE / sizeof(item_t);
    if (count > max_capacity - queue->size) {
        return QUEUE_ERR;
    }

    size_t needed = queue->size + count;
    size_t new_capacity =
        queue->capacity ? queue->capacity : QUEUE_BASE_CAPACITY;

    while (new_capacity < needed) {
        if (new_capacity > max_capacity / QUEUE_GROWTH_FACTOR) {
            return QUEUE_ERR;
        }

        new_capacity *= QUEUE_GROWTH_FACTOR;
    }

    item_t* new_data =
        realloc((void*)queue->data, new_capacity * sizeof(item_t));
    if (!new_data) {
        return QUEUE_ERR;
    }

    if (queue->tail + queue->size > queue->capacity) {
        size_t wrapped = queue->tail + queue->size - queue->capacity;

        memcpy((void*)(new_data + queue->capacity), (const void*)new_data,
               wrapped * sizeof(item_t));
    }

    queue->data     = new_data;
    queue->capacity = new_capacity;
    return QUEUE_OK;
}

void queue_enqueue_unchecked(queue_t* queue, item_t item) {
    assert(queue && queue->size < queue->capacity);

    size_t idx       = (queue->tail + queue->size) & (queue->capacity - 1);
    queue->data[idx] = item;
    ++queue->size;
}

int queue_enqueue(queue_t* queue, item_t item) {
    if (!queue || !queue_ensure_capacity(queue, 1)) {
        return QUEUE_ERR;
    }

    queue_enqueue_unchecked(queue, item);
    return QUEUE_OK;
}

item_t queue_peek_unchecked(const queue_t* queue) {
    assert(queue && queue->size);

    return queue->data[queue->tail];
}

int queue_peek(const queue_t* queue, item_t* out_item) {
    if (!queue || !queue->size) {
        *out_item = item_zero();
        return QUEUE_ERR;
    }

    *out_item = queue_peek_unchecked(queue);
    return QUEUE_OK;
}

item_t queue_dequeue_unchecked(queue_t* queue) {
    assert(queue && queue->size);

    item_t item = queue->data[queue->tail];
    queue->tail = (queue->tail + 1) & (queue->capacity - 1);
    --queue->size;

    return item;
}

int queue_dequeue(queue_t* queue, item_t* out_item) {
    if (!queue || !queue->size) {
        *out_item = item_zero();
        return QUEUE_ERR;
    }

    *out_item = queue_dequeue_unchecked(queue);
    return QUEUE_OK;
}

void queue_print(const queue_t* queue) {
    if (!queue) {
        return;
    }

    printf("queue: [");
    for (size_t i = 0; i < queue->size; ++i) {
        if (i) {
            printf(", ");
        }

        item_print(&queue->data[(queue->tail + i) & (queue->capacity - 1)]);
    }
    printf("] (size: %zu)\n", queue->size);
}

// >----------------------------------------------------------------------- main

void test_enqueue_dequeue(void) {
    printf("---- enqueue dequeue\n");

    queue_t queue = queue_make();
    item_t item   = item_zero();

    queue_enqueue(&queue, item_make(10));
    queue_enqueue(&queue, item_make(20));
    queue_enqueue(&queue, item_make(30));
    queue_print(&queue);

    queue_peek(&queue, &item);
    printf("peek: ");
    item_print(&item);
    printf("\n");

    queue_dequeue(&queue, &item);
    printf("dequeue: ");
    item_print(&item);
    printf("\n");

    queue_print(&queue);
    queue_destroy(&queue);
    printf("\n");
}

void test_empty(void) {
    printf("---- empty\n");

    queue_t queue = queue_make();
    item_t item   = item_zero();

    queue_print(&queue);
    printf("peek: %s\n", queue_peek(&queue, &item) ? "ok" : "err");
    printf("dequeue: %s\n", queue_dequeue(&queue, &item) ? "ok" : "err");

    queue_destroy(&queue);
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    queue_t queue = queue_make();
    item_t item   = item_zero();

    for (int i = 0; i < 4; ++i) {
        queue_enqueue(&queue, item_make(i));
    }
    queue_dequeue(&queue, &item);
    queue_enqueue(&queue, item_make(4));
    queue_print(&queue);

    if (queue_ensure_capacity(&queue, 3)) {
        queue_enqueue_unchecked(&queue, item_make(5));
        queue_enqueue_unchecked(&queue, item_make(6));
        queue_enqueue_unchecked(&queue, item_make(7));
    }
    queue_print(&queue);

    item = queue_dequeue_unchecked(&queue);
    printf("dequeue: ");
    item_print(&item);
    printf("\n");

    queue_print(&queue);
    queue_destroy(&queue);
    printf("\n");
}

int main(void) {
    test_enqueue_dequeue();
    test_empty();
    test_unchecked();

    return 0;
}
