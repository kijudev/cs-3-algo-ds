/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
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

void insertion_sort_unchecked(item_t* array, size_t size) {
    assert(array);

    for (size_t i = 1; i < size; ++i) {
        item_t value = array[i];
        size_t j     = i;

        while (j > 0 && item_less(&value, &array[j - 1])) {
            array[j] = array[j - 1];
            --j;
        }

        array[j] = value;
    }
}

void insertion_sort(item_t* array, size_t size) {
    if (!array || size < 2) {
        return;
    }

    insertion_sort_unchecked(array, size);
}

// >----------------------------------------------------------------------- main

void test_sort(void) {
    printf("---- sort\n");

    item_t array[] = {
        item_make(6), item_make(2), item_make(7), item_make(3),
        item_make(5), item_make(4), item_make(1),
    };
    size_t size = sizeof(array) / sizeof(array[0]);

    printf("before: ");
    items_print(array, size);

    insertion_sort(array, size);

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

    insertion_sort_unchecked(array, size);

    printf("after: ");
    items_print(array, size);
    printf("\n");
}

int main(void) {
    test_sort();
    test_unchecked();

    return 0;
}
