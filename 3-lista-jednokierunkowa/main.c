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

item_t item_make(int value) { return (item_t){value}; }

typedef struct {
    item_t item;
    struct node_t* next;
} node_t;

node_t node_make(item_t item) { return (node_t){item, NULL}; }

typedef struct {
    size_t size;
    node_t* data;
} bucket_t;

bucket_t bucket_make(size_t size) {
    return (bucket_t){
        .size = size,
        .data = (node_t*)malloc(size * sizeof(item_t)),
    };
}

void bucket_free(bucket_t* bucket) {
    free((void*)bucket->data);
    *bucket = (bucket_t){0};
}

typedef struct {
    node_t* dummy;
    size_t count;
    bucket_t buckets[32];
} stack_t;

stack_t list_make(void) {
    stack_t list = {0};

    list.buckets[0] = bucket_make(16);
    list.dummy      = list.buckets[0].data;
    list.count      = 0;

    return list;
}

void list_free(stack_t* list) {
    for (size_t i = 0; i < list->count; ++i) {
        bucket_free(&list->buckets[i]);
    }

    *list = (stack_t){0};
}

int main(void) { return 0; }
