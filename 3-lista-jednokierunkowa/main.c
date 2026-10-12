/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 *
 * Singly linked list is implemented using an exponential array.
 * This way nodes of the list behave normally (pointers remain stable).
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
} item_t;

typedef struct node_t {
    item_t item;
    struct node_t* next;
} node_t;

typedef struct bucket_t {
    size_t size;
    node_t* data;
    struct bucket_t* next;
} bucket_t;

typedef struct {
    node_t* head;
    node_t* next;
    bucket_t* bucket_tail;
    bucket_t* bucket_head;
} list_t;

bucket_t bucket_make(size_t size) {
    return (bucket_t){
        .size = size,
        .data = malloc(size * sizeof(node_t)),
        .next = NULL,
    };
}

bucket_t* bucket_new(size_t size) {
    bucket_t* bucket = (bucket_t*)malloc(sizeof(bucket_t));

    *bucket = bucket_make(size);
    return bucket;
}

void bucket_free(bucket_t* bucket) { free((void*)bucket->data); }

list_t list_make(void) {
    bucket_t* bucket = bucket_new(16);
    return (list_t){
        .head        = NULL,
        .next        = &bucket->data[0],
        .bucket_tail = bucket,
        .bucket_head = bucket,
    };
}

void list_free(list_t* list) {
    bucket_t* curr = list->bucket_tail;

    while (curr) {
        bucket_t* next = curr->next;
        bucket_free(curr);
        free((void*)curr);
        curr = next;
    }

    *list = (list_t){0};
}

node_t* list_alloc(list_t* list) {
    bucket_t* bucket = list->bucket_head;

    if (list->head >= (bucket->data + bucket->size)) {
        bucket_t* new_bucket = bucket_new(bucket->size * 2);

        bucket->next      = new_bucket;
        list->bucket_head = new_bucket;
        list->next        = new_bucket->data;
    }

    node_t* node = list->next++;

    *node = (node_t){0};
    return node;
}

node_t* list_append(list_t* list, item_t item) {
    node_t* curr = list->head;
    if (!curr) {
        list->head       = list_alloc(list);
        list->head->item = item;
        return list->head;
    }

    while (curr && curr->next) {
        curr = curr->next;
    }

    node_t* node = list_alloc(list);
    node->item   = item;
    curr->next   = node;

    return node;
}

node_t* list_prepend(list_t* list, item_t item) {
    node_t* node = list_alloc(list);
    node->item   = item;

    node_t* head = list->head;
    list->head   = node;
    node->next   = head;

    return node;
}

int main(void) {
    list_t list = list_make();

    list_append(&list, (item_t){1});
    list_append(&list, (item_t){2});
    list_append(&list, (item_t){3});
    list_append(&list, (item_t){4});
    list_prepend(&list, (item_t){5});
    list_prepend(&list, (item_t){6});

    node_t* curr = list.head;
    while (curr) {
        printf("item: %d\n", curr->item.value);
        curr = curr->next;
    }

    list_free(&list);
    return 0;
}
