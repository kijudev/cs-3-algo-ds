/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 *
 * Circular singly linked list is implemented using an exponential array.
 * This way nodes of the list behave normally (pointers remain stable).
 * Only the tail is stored, the head is always tail->next.
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
} node_t;

typedef struct bucket_t {
    size_t size;
    node_t* data;
    struct bucket_t* next;
} bucket_t;

typedef struct {
    node_t* tail;
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
    return (list_t){
        .tail          = NULL,
        .size          = 0,
        .next          = &bucket->data[0],
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
    return list->tail ? list->tail->next : NULL;
}

node_t* list_tail(const list_t* list) { return list->tail; }

size_t list_size(const list_t* list) { return list->size; }

int list_is_empty(const list_t* list) { return list->size == 0; }

node_t* list_link(list_t* list, item_t item) {
    node_t* node = list_alloc(list);
    node->item   = item;

    if (list->tail) {
        node->next       = list->tail->next;
        list->tail->next = node;
    } else {
        node->next = node;
        list->tail = node;
    }

    list->size++;
    return node;
}

node_t* list_append(list_t* list, item_t item) {
    node_t* node = list_link(list, item);
    list->tail   = node;
    return node;
}

node_t* list_prepend(list_t* list, item_t item) {
    return list_link(list, item);
}

node_t* list_insert_after(list_t* list, node_t* node, item_t item) {
    assert(node);

    node_t* new_node = list_alloc(list);
    new_node->item   = item;
    new_node->next   = node->next;
    node->next       = new_node;

    if (list->tail == node) {
        list->tail = new_node;
    }
    list->size++;

    return new_node;
}

void list_rotate(list_t* list) {
    if (list->tail) {
        list->tail = list->tail->next;
    }
}

node_t* list_find(const list_t* list, item_t item) {
    node_t* curr = list_head(list);
    for (size_t i = 0; i < list->size; i++) {
        if (item_eq(item, curr->item)) {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

void list_unlink(list_t* list, node_t* prev, node_t* curr) {
    if (prev == curr) {
        list->tail = NULL;
    } else {
        prev->next = curr->next;
        if (list->tail == curr) {
            list->tail = prev;
        }
    }

    list->size--;
    list_release(list, curr);
}

int list_remove(list_t* list, node_t* node) {
    if (!node || !list->tail) {
        return 0;
    }

    node_t* prev = list->tail;
    for (size_t i = 0; i < list->size; i++) {
        node_t* curr = prev->next;

        if (curr == node) {
            list_unlink(list, prev, curr);
            return 1;
        }

        prev = curr;
    }

    return 0;
}

int list_find_remove(list_t* list, item_t item) {
    return list_remove(list, list_find(list, item));
}

size_t list_find_remove_all(list_t* list, item_t item) {
    if (!list->tail) {
        return 0;
    }

    size_t removed = 0;
    size_t size    = list->size;
    node_t* prev   = list->tail;
    node_t* curr   = list->tail->next;

    for (size_t i = 0; i < size; i++) {
        node_t* next = curr->next;

        if (item_eq(item, curr->item)) {
            list_unlink(list, prev, curr);
            removed++;
        } else {
            prev = curr;
        }

        curr = next;
    }

    return removed;
}

int list_pop_front(list_t* list, item_t* out) {
    if (!list->tail) {
        return 0;
    }

    if (out) {
        *out = list->tail->next->item;
    }

    list_unlink(list, list->tail, list->tail->next);
    return 1;
}

int list_pop_back(list_t* list, item_t* out) {
    if (!list->tail) {
        return 0;
    }

    if (out) {
        *out = list->tail->item;
    }

    node_t* prev = list->tail;
    while (prev->next != list->tail) {
        prev = prev->next;
    }

    list_unlink(list, prev, list->tail);
    return 1;
}

void list_clear(list_t* list) {
    node_t* curr = list_head(list);
    for (size_t i = 0; i < list->size; i++) {
        node_t* next = curr->next;
        list_release(list, curr);
        curr = next;
    }

    list->tail = NULL;
    list->size = 0;
}

void list_print(const list_t* list) {
    assert(list);

    node_t* curr = list_head(list);
    for (size_t i = 0; i < list->size; i++) {
        item_print(curr->item);
        curr = curr->next;
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

    printf("---- recycle slots of removed items:\n");
    list_append(&list, (item_t){42});
    list_prepend(&list, (item_t){420});
    list_print(&list);
    printf("head: %d, tail: %d\n\n", list_head(&list)->item.value,
           list_tail(&list)->item.value);

    printf("---- insert after head:\n");
    list_insert_after(&list, list_head(&list), (item_t){7});
    list_print(&list);
    printf("\n");

    printf("---- rotate:\n");
    list_rotate(&list);
    list_print(&list);
    printf("head: %d, tail: %d\n\n", list_head(&list)->item.value,
           list_tail(&list)->item.value);

    printf("---- walk past the tail:\n");
    node_t* curr = list_head(&list);
    for (size_t i = 0; i < 2 * list_size(&list); i++) {
        printf("%d ", curr->item.value);
        curr = curr->next;
    }
    printf("\n\n");

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
