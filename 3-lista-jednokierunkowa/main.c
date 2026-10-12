/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 *
 * Singly linked list is implemented using an exponential array.
 * This way nodes of the list behave normally (pointers remain stable).
 */

#include <assert.h>
#include <stdlib.h>

#define BUCKET_MAX_COUNT   (32)
#define BUCKET_SIZE_OFFSET (4)

#include <stddef.h>

typedef struct node_t {
    int value;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* buckets[BUCKET_MAX_COUNT];
    node_t* next_node;
    node_t* free_node;
    size_t bucket_count;
} list_t;

size_t list_calc_bucket_size(size_t idx) {
    return 1 << (idx + BUCKET_SIZE_OFFSET);
}

void list_add_bucket(list_t* list) {
    assert(list->bucket_count <= BUCKET_MAX_COUNT);

    list->buckets[list->bucket_count] =
        malloc(list_calc_bucket_size(list->bucket_count) * sizeof(node_t));
    ++list->bucket_count;
}

list_t list_make(void) {
    list_t list = {0};

    list.buckets[0] =
        malloc(list_calc_bucket_size(list.bucket_count++) * sizeof(node_t));
    list.next_node = list.buckets[0];

    return list;
}

node_t* list_allocate(list_t* list) {
    if (list->free_node) {
        node_t* node = list->free_node;
        node_t* next = node->next;

        list->free_node = next;
        *node           = (node_t){0};

        return node;
    }

    node_t* bucket     = list->buckets[list->bucket_count - 1];
    size_t bucket_size = list_calc_bucket_size(list->bucket_count - 1);

    if (list->next_node >= (bucket + bucket_size)) {
        list_add_bucket(list);
        list->next_node = list->buckets[list->bucket_count - 1];
    }

    node_t* node = list->next_node;
    ++list->next_node;

    *node = (node_t){0};
    return node;
}

int list_remove(list_t* list, node_t* node) {}

int main(void) { return 0; }
