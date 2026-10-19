/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// >----------------------------------------------------------------------- item

typedef struct {
    int value;
} item_t;

item_t item_make(int value) { return (item_t){value}; }
item_t item_zero(void) { return (item_t){0}; }

int item_eq(const item_t* a, const item_t* b) { return a->value == b->value; }

void item_print(const item_t* item) { printf("%d", item->value); }

// >----------------------------------------------------------------------- node

typedef struct node_t {
    item_t item;
    struct node_t* next;
} node_t;

// >--------------------------------------------------------------------- bucket

typedef struct bucket_t {
    size_t size;
    node_t* data;
    struct bucket_t* next;
} bucket_t;

bucket_t* bucket_new(size_t size) {
    bucket_t* bucket = malloc(sizeof(bucket_t));
    if (!bucket) {
        return NULL;
    }

    node_t* data = malloc(size * sizeof(node_t));
    if (!data) {
        free((void*)bucket);
        return NULL;
    }

    *bucket = (bucket_t){
        .size = size,
        .data = data,
        .next = NULL,
    };

    return bucket;
}

void bucket_destroy(bucket_t* bucket) {
    if (!bucket) {
        return;
    }

    free((void*)bucket->data);
    free((void*)bucket);
}

// >----------------------------------------------------------------------- list

#define LIST_OK            (1)
#define LIST_ERR           (0)
#define LIST_BASE_CAPACITY (16)
#define LIST_GROWTH_FACTOR (2)
#define LIST_MAX_SIZE      (SIZE_MAX)

typedef struct {
    node_t* head;
    node_t* tail;
    size_t size;
    node_t* next;
    node_t* free;
    bucket_t* buckets_first;
    bucket_t* buckets_last;
} list_t;

list_t list_make(void) { return (list_t){0}; }

void list_destroy(list_t* list) {
    if (!list) {
        return;
    }

    bucket_t* curr = list->buckets_first;

    while (curr) {
        bucket_t* next = curr->next;
        bucket_destroy(curr);
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

    if (!bucket || list->next >= (bucket->data + bucket->size)) {
        size_t size = LIST_BASE_CAPACITY;

        if (bucket) {
            if (bucket->size >
                LIST_MAX_SIZE / sizeof(node_t) / LIST_GROWTH_FACTOR) {
                return NULL;
            }

            size = bucket->size * LIST_GROWTH_FACTOR;
        }

        bucket_t* new_bucket = bucket_new(size);
        if (!new_bucket) {
            return NULL;
        }

        if (bucket) {
            bucket->next = new_bucket;
        } else {
            list->buckets_first = new_bucket;
        }

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
    if (!list) {
        return NULL;
    }

    return list->head;
}

node_t* list_tail(const list_t* list) {
    if (!list) {
        return NULL;
    }

    return list->tail;
}

size_t list_size(const list_t* list) {
    if (!list) {
        return 0;
    }

    return list->size;
}

int list_is_empty(const list_t* list) {
    if (!list) {
        return 1;
    }

    return list->size == 0;
}

node_t* list_append(list_t* list, item_t item) {
    if (!list) {
        return NULL;
    }

    node_t* node = list_alloc(list);
    if (!node) {
        return NULL;
    }

    node->item = item;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    list->size++;

    return node;
}

node_t* list_prepend(list_t* list, item_t item) {
    if (!list) {
        return NULL;
    }

    node_t* node = list_alloc(list);
    if (!node) {
        return NULL;
    }

    node->item = item;
    node->next = list->head;

    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }
    list->size++;

    return node;
}

node_t* list_insert_after(list_t* list, node_t* node, item_t item) {
    if (!list || !node) {
        return NULL;
    }

    node_t* new_node = list_alloc(list);
    if (!new_node) {
        return NULL;
    }

    new_node->item = item;
    new_node->next = node->next;
    node->next     = new_node;

    if (list->tail == node) {
        list->tail = new_node;
    }
    list->size++;

    return new_node;
}

node_t* list_find(const list_t* list, item_t item) {
    if (!list) {
        return NULL;
    }

    node_t* curr = list->head;
    while (curr) {
        if (item_eq(&item, &curr->item)) {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

void list_unlink(list_t* list, node_t* prev, node_t* curr) {
    if (prev) {
        prev->next = curr->next;
    } else {
        list->head = curr->next;
    }

    if (list->tail == curr) {
        list->tail = prev;
    }

    list->size--;
    list_release(list, curr);
}

int list_remove(list_t* list, node_t* node) {
    if (!list || !node) {
        return LIST_ERR;
    }

    node_t* prev = NULL;
    node_t* curr = list->head;

    while (curr) {
        if (curr == node) {
            list_unlink(list, prev, curr);
            return LIST_OK;
        }

        prev = curr;
        curr = curr->next;
    }

    return LIST_ERR;
}

int list_find_remove(list_t* list, item_t item) {
    return list_remove(list, list_find(list, item));
}

size_t list_find_remove_all(list_t* list, item_t item) {
    if (!list) {
        return 0;
    }

    size_t removed = 0;
    node_t* prev   = NULL;
    node_t* curr   = list->head;

    while (curr) {
        node_t* next = curr->next;

        if (item_eq(&item, &curr->item)) {
            list_unlink(list, prev, curr);
            removed++;
        } else {
            prev = curr;
        }

        curr = next;
    }

    return removed;
}

item_t list_pop_front_unchecked(list_t* list) {
    assert(list && list->size);

    item_t item = list->head->item;
    list_unlink(list, NULL, list->head);

    return item;
}

int list_pop_front(list_t* list, item_t* out_item) {
    if (!list || !list->size) {
        *out_item = item_zero();
        return LIST_ERR;
    }

    *out_item = list_pop_front_unchecked(list);
    return LIST_OK;
}

item_t list_pop_back_unchecked(list_t* list) {
    assert(list && list->size);

    item_t item  = list->tail->item;
    node_t* prev = NULL;
    node_t* curr = list->head;

    while (curr != list->tail) {
        prev = curr;
        curr = curr->next;
    }

    list_unlink(list, prev, curr);
    return item;
}

int list_pop_back(list_t* list, item_t* out_item) {
    if (!list || !list->size) {
        *out_item = item_zero();
        return LIST_ERR;
    }

    *out_item = list_pop_back_unchecked(list);
    return LIST_OK;
}

void list_clear(list_t* list) {
    if (!list) {
        return;
    }

    node_t* curr = list->head;
    while (curr) {
        node_t* next = curr->next;
        list_release(list, curr);
        curr = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_print(const list_t* list) {
    if (!list) {
        return;
    }

    printf("list: [");

    node_t* curr = list->head;
    while (curr) {
        if (curr != list->head) {
            printf(", ");
        }

        item_print(&curr->item);
        curr = curr->next;
    }

    printf("] (size: %zu)\n", list->size);
}

// >----------------------------------------------------------------------- main

void test_insert(void) {
    printf("---- insert\n");

    list_t list = list_make();

    list_append(&list, item_make(1));
    list_prepend(&list, item_make(2));
    list_append(&list, item_make(3));
    list_insert_after(&list, list_head(&list), item_make(7));
    list_print(&list);

    list_destroy(&list);
    printf("\n");
}

void test_find_remove(void) {
    printf("---- find remove\n");

    list_t list = list_make();

    for (int i = 0; i < 6; ++i) {
        list_append(&list, item_make(i % 3));
    }
    list_print(&list);

    list_find_remove(&list, item_make(0));
    list_print(&list);

    printf("removed: %zu\n", list_find_remove_all(&list, item_make(1)));
    list_print(&list);

    printf("found 7: %d\n", list_find(&list, item_make(7)) != NULL);

    list_destroy(&list);
    printf("\n");
}

void test_pop(void) {
    printf("---- pop\n");

    list_t list = list_make();
    item_t item = item_zero();

    for (int i = 1; i <= 3; ++i) {
        list_append(&list, item_make(i));
    }
    list_print(&list);

    list_pop_front(&list, &item);
    printf("pop front: ");
    item_print(&item);
    printf("\n");

    list_pop_back(&list, &item);
    printf("pop back: ");
    item_print(&item);
    printf("\n");

    list_print(&list);

    list_pop_front(&list, &item);
    printf("pop empty: %s\n", list_pop_front(&list, &item) ? "ok" : "err");

    list_destroy(&list);
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    list_t list = list_make();
    item_t item = item_zero();

    for (int i = 1; i <= 3; ++i) {
        list_append(&list, item_make(i));
    }
    list_print(&list);

    item = list_pop_front_unchecked(&list);
    printf("pop front: ");
    item_print(&item);
    printf("\n");

    item = list_pop_back_unchecked(&list);
    printf("pop back: ");
    item_print(&item);
    printf("\n");

    list_print(&list);
    list_destroy(&list);
    printf("\n");
}

void test_grow_clear(void) {
    printf("---- grow clear\n");

    list_t list = list_make();

    for (int i = 0; i < 40; ++i) {
        list_append(&list, item_make(i));
    }
    printf("size: %zu\n", list_size(&list));

    list_clear(&list);
    list_print(&list);

    list_append(&list, item_make(42));
    list_print(&list);

    list_destroy(&list);
    printf("\n");
}

int main(void) {
    test_insert();
    test_find_remove();
    test_pop();
    test_unchecked();
    test_grow_clear();

    return 0;
}
