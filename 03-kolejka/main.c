/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
} item_t;

typedef struct {
    size_t tail;
    size_t size;
    size_t capacity;
    item_t* data;
} queue_t;

queue_t queue_make(void) {
    queue_t queue = {
        .tail     = 0,
        .size     = 0,
        .capacity = 0,
        .data     = NULL,
    };

    return queue;
}

void queue_free(queue_t* queue) {
    free((void*)queue->data);
    *queue = (queue_t){0};
}

int queue_is_full(const queue_t* queue) {
    return queue->size == queue->capacity;
}

int queue_is_empty(const queue_t* queue) { return queue->size == 0; }

size_t queue_size(const queue_t* queue) { return queue->size; }

void queue_enqueue(queue_t* queue, item_t item) {
    if (!queue->data) {
        queue->capacity = 2;
        queue->data     = (item_t*)malloc(queue->capacity * sizeof(item_t));
    } else if (queue_is_full(queue)) {
        size_t new_capacity = queue->capacity * 2;

        if (queue->tail + queue->size <= queue->capacity) {
            queue->data =
                realloc((void*)queue->data, new_capacity * sizeof(item_t));
        } else {
            item_t* old_data  = queue->data;
            item_t* new_data  = malloc(new_capacity);
            size_t right_size = queue->capacity - queue->tail;
            size_t left_size  = queue->size - right_size;

            memcpy((void*)new_data, (const void*)(old_data + queue->tail),
                   right_size * sizeof(item_t));

            memcpy((void*)(new_data + right_size), (const void*)old_data,
                   left_size * sizeof(item_t));

            free((void*)old_data);
            queue->tail = 0;
            queue->data = new_data;
        }

        queue->capacity = new_capacity;
    }

    size_t idx       = (queue->tail + queue->size) & (queue->capacity - 1);
    queue->data[idx] = item;
    ++queue->size;
}

item_t queue_dequeue(queue_t* queue) {
    if (queue->size == 0) {
        return (item_t){0};
    }

    size_t next_tail = (queue->tail + 1) & (queue->capacity - 1);
    item_t item      = queue->data[queue->tail];
    queue->tail      = next_tail;
    --queue->size;

    return item;
}

item_t queue_peek(const queue_t* queue) {
    if (queue->size == 0) {
        return (item_t){0};
    }

    return queue->data[queue->tail];
}

int main(void) {
    queue_t queue = queue_make();
    queue_enqueue(&queue, (item_t){10});
    queue_enqueue(&queue, (item_t){20});
    queue_enqueue(&queue, (item_t){30});
    queue_enqueue(&queue, (item_t){40});

    while (queue.size > 0) {
        item_t item = queue_dequeue(&queue);
        printf("item: %d\n", item.value);
    }

    queue_free(&queue);

    return 0;
}
