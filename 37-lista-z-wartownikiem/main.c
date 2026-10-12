/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 *
 * Doubly linked list with a sentinel is implemented using an exponential array.
 * This way nodes of the list behave normally (pointers remain stable).
 * The sentinel is the first node of the pool, sentinel->next is the head and
 * sentinel->prev is the tail, so linking and unlinking need no NULL checks.
 * Shares the underlying architecture with the other list exercises.
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
    struct node_t* prev;
} node_t;

typedef struct bucket_t {
    size_t size;
    node_t* data;
    struct bucket_t* next;
} bucket_t;

typedef struct {
    node_t* sentinel;
    size_t size;
    node_t* next;
    node_t* free;
    bucket_t* buckets_first;
    bucket_t* buckets_last;
} list_t;

int item_eq(item_t a, item_t b) { return a.value == b.value; }

void item_print(item_t item) { printf("item { value: %d }\n", item.value); }

void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "out of memory\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

bucket_t bucket_make(size_t size) {
    return (bucket_t){
        .size = size,
        .data = xmalloc(size * sizeof(node_t)),
        .next = NULL,
    };
}

bucket_t* bucket_new(size_t size) {
    bucket_t* bucket = (bucket_t*)xmalloc(sizeof(bucket_t));

    *bucket = bucket_make(size);
    return bucket;
}

void bucket_free(bucket_t* bucket) { free((void*)bucket->data); }

list_t list_make(void) {
    bucket_t* bucket = bucket_new(16);
    node_t* sentinel = &bucket->data[0];

    *sentinel = (node_t){
        .item = {0},
        .next = sentinel,
        .prev = sentinel,
    };

    return (list_t){
        .sentinel      = sentinel,
        .size          = 0,
        .next          = &bucket->data[1],
        .free          = NULL,
        .buckets_first = bucket,
        .buckets_last  = bucket,
    };
}

void list_free(list_t* list) {
    bucket_t* curr = list->buckets_first;

    while (curr) {
        bucket_t* next = curr->next;
        bucket_free(curr);
        free((void*)curr);
        curr = next;
    }

    *list = (list_t){0};
}

node_t* list_alloc(list_t* list) {
    if (list->free) {
        node_t* node = list->free;
        list->free   = node->next;
        *node        = (node_t){0};
        return node;
    }

    bucket_t* bucket = list->buckets_last;

    if (list->next >= (bucket->data + bucket->size)) {
        bucket_t* new_bucket = bucket_new(bucket->size * 2);

        bucket->next       = new_bucket;
        list->buckets_last = new_bucket;
        list->next         = new_bucket->data;
    }

    node_t* node = list->next++;

    *node = (node_t){0};
    return node;
}

void list_release(list_t* list, node_t* node) {
    *node      = (node_t){0};
    node->next = list->free;
    list->free = node;
}

node_t* list_head(const list_t* list) {
    return list->size ? list->sentinel->next : NULL;
}

node_t* list_tail(const list_t* list) {
    return list->size ? list->sentinel->prev : NULL;
}

size_t list_size(const list_t* list) { return list->size; }

int list_is_empty(const list_t* list) { return list->size == 0; }

node_t* list_link(list_t* list, node_t* prev, node_t* next, item_t item) {
    node_t* node = list_alloc(list);
    node->item   = item;
    node->prev   = prev;
    node->next   = next;
    prev->next   = node;
    next->prev   = node;

    list->size++;
    return node;
}

node_t* list_append(list_t* list, item_t item) {
    return list_link(list, list->sentinel->prev, list->sentinel, item);
}

node_t* list_prepend(list_t* list, item_t item) {
    return list_link(list, list->sentinel, list->sentinel->next, item);
}

node_t* list_insert_after(list_t* list, node_t* node, item_t item) {
    assert(node);
    return list_link(list, node, node->next, item);
}

node_t* list_insert_before(list_t* list, node_t* node, item_t item) {
    assert(node);
    return list_link(list, node->prev, node, item);
}

node_t* list_find(const list_t* list, item_t item) {
    list->sentinel->item = item;

    node_t* curr = list->sentinel->next;
    while (!item_eq(item, curr->item)) {
        curr = curr->next;
    }

    return curr == list->sentinel ? NULL : curr;
}

int list_remove(list_t* list, node_t* node) {
    if (!node || node == list->sentinel) {
        return 0;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    list->size--;
    list_release(list, node);
    return 1;
}

int list_find_remove(list_t* list, item_t item) {
    return list_remove(list, list_find(list, item));
}

size_t list_find_remove_all(list_t* list, item_t item) {
    size_t removed = 0;
    node_t* curr   = list->sentinel->next;

    while (curr != list->sentinel) {
        node_t* next = curr->next;

        if (item_eq(item, curr->item)) {
            list_remove(list, curr);
            removed++;
        }

        curr = next;
    }

    return removed;
}

int list_pop_front(list_t* list, item_t* out) {
    if (!list->size) {
        return 0;
    }

    if (out) {
        *out = list->sentinel->next->item;
    }

    return list_remove(list, list->sentinel->next);
}

int list_pop_back(list_t* list, item_t* out) {
    if (!list->size) {
        return 0;
    }

    if (out) {
        *out = list->sentinel->prev->item;
    }

    return list_remove(list, list->sentinel->prev);
}

void list_clear(list_t* list) {
    node_t* curr = list->sentinel->next;
    while (curr != list->sentinel) {
        node_t* next = curr->next;
        list_release(list, curr);
        curr = next;
    }

    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    list->size           = 0;
}

void list_print(const list_t* list) {
    assert(list);

    node_t* curr = list->sentinel->next;
    while (curr != list->sentinel) {
        item_print(curr->item);
        curr = curr->next;
    }
}

void list_print_reverse(const list_t* list) {
    assert(list);

    node_t* curr = list->sentinel->prev;
    while (curr != list->sentinel) {
        item_print(curr->item);
        curr = curr->prev;
    }
}

int main(void) {
    list_t list = list_make();

    printf("---- add items:\n");
    list_append(&list, (item_t){1});
    list_prepend(&list, (item_t){2});
    list_append(&list, (item_t){3});
    list_print(&list);
    printf("size: %zu\n\n", list_size(&list));

    printf("---- find and remove items:\n");
    list_find_remove(&list, (item_t){2});
    list_find_remove(&list, (item_t){1});
    list_print(&list);
    printf("size: %zu\n\n", list_size(&list));

    printf("---- find missing item:\n");
    printf("found: %d\n\n", list_find(&list, (item_t){99}) != NULL);

    printf("---- recycle slots of removed items:\n");
    list_append(&list, (item_t){42});
    list_prepend(&list, (item_t){420});
    list_print(&list);
    printf("head: %d, tail: %d\n\n", list_head(&list)->item.value,
           list_tail(&list)->item.value);

    printf("---- insert after head and before tail:\n");
    list_insert_after(&list, list_head(&list), (item_t){7});
    list_insert_before(&list, list_tail(&list), (item_t){8});
    list_print(&list);
    printf("\n");

    printf("---- print in reverse:\n");
    list_print_reverse(&list);
    printf("\n");

    printf("---- grow past the first bucket and remove all 5s:\n");
    for (int i = 0; i < 40; i++) {
        list_append(&list, (item_t){i % 2 ? 5 : i});
    }
    printf("size before: %zu\n", list_size(&list));
    printf("removed: %zu\n", list_find_remove_all(&list, (item_t){5}));
    printf("size after: %zu\n\n", list_size(&list));

    printf("---- pop front and back:\n");
    item_t item;
    list_pop_front(&list, &item);
    item_print(item);
    list_pop_back(&list, &item);
    item_print(item);
    printf("head: %d, tail: %d\n\n", list_head(&list)->item.value,
           list_tail(&list)->item.value);

    printf("---- clear:\n");
    list_clear(&list);
    printf("size: %zu, empty: %d\n", list_size(&list), list_is_empty(&list));

    list_free(&list);
    return 0;
}
