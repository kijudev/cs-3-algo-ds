/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.13
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

// >----------------------------------------------------------------------- item

typedef struct {
    int value;
} item_t;

item_t item_make(int value) { return (item_t){value}; }
item_t item_zero(void) { return (item_t){0}; }

int item_eq(const item_t* a, const item_t* b) { return a->value == b->value; }
int item_less(const item_t* a, const item_t* b) { return a->value < b->value; }

void item_swap(item_t* a, item_t* b) {
    item_t temp = *a;
    *a          = *b;
    *b          = temp;
}

void item_print(const item_t* item) { printf("%d", item->value); }

void items_print(const item_t* items, size_t size) {
    if (!items) {
        return;
    }

    printf("[");
    for (size_t i = 0; i < size; ++i) {
        if (i) {
            printf(", ");
        }

        item_print(&items[i]);
    }
    printf("]\n");
}

// >----------------------------------------------------------------- algorithms

size_t quick_sort_partition(item_t* array, size_t a, size_t b) {
    assert(a <= b);

    item_t p     = array[a];
    size_t left  = a;
    size_t right = b;

    while (left <= right) {
        if (!item_less(&p, &array[left])) {
            ++left;
        } else if (item_less(&p, &array[right])) {
            --right;
        } else {
            item_swap(&array[left], &array[right]);
            ++left;
            --right;
        }
    }

    item_swap(&array[a], &array[right]);
    return right;
}

void quick_sort_rec(item_t* array, size_t a, size_t b) {
    if (a >= b) {
        return;
    }

    size_t p = quick_sort_partition(array, a, b);

    if (p > a) {
        quick_sort_rec(array, a, p - 1);
    }

    quick_sort_rec(array, p + 1, b);
}

void quick_sort_unchecked(item_t* array, size_t size) {
    assert(array && size);

    quick_sort_rec(array, 0, size - 1);
}

void quick_sort(item_t* array, size_t size) {
    if (!array || size < 2) {
        return;
    }

    quick_sort_unchecked(array, size);
}

// >----------------------------------------------------------------------- main

void test_sort(void) {
    printf("---- sort\n");

    item_t array[] = {
        item_make(3),  item_make(2),  item_make(1), item_make(42),
        item_make(43), item_make(55), item_make(0),
    };
    size_t size = sizeof(array) / sizeof(array[0]);

    printf("before: ");
    items_print(array, size);

    quick_sort(array, size);

    printf("after: ");
    items_print(array, size);
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    item_t array[] = {
        item_make(5), item_make(1), item_make(4), item_make(1),
        item_make(5), item_make(9), item_make(2), item_make(6),
    };
    size_t size = sizeof(array) / sizeof(array[0]);

    printf("before: ");
    items_print(array, size);

    quick_sort_unchecked(array, size);

    printf("after: ");
    items_print(array, size);
    printf("\n");
}

int main(void) {
    test_sort();
    test_unchecked();

    return 0;
}
