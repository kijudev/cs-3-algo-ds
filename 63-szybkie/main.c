/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.13
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void swap(int* array, int a, int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

int quick_sort_partition(int* array, int a, int b) {
    assert(a <= b);

    int p     = array[a];
    int left  = a;
    int right = b;

    while (left <= right) {
        if (array[left] <= p) {
            ++left;
        } else if (array[right] > p) {
            --right;
        } else {
            swap(array, left, right);
            ++left;
            --right;
        }
    }

    swap(array, a, right);
    return a;
}

void quick_sort_rec(int* array, int a, int b) {
    if (a >= b) {
        return;
    }

    int p = quick_sort_partition(array, a, b);
    quick_sort_rec(array, a, p - 1);
    quick_sort_rec(array, p + 1, b);
}

void quick_sort(int array[], int size) {
    if (!size) {
        return;
    }

    quick_sort_rec(array, 0, size - 1);
}

int main(void) {
    int array[] = {3, 2, 1, 42, 43, 55, 0};
    int size    = sizeof(array) / sizeof(int);

    quick_sort(array, size);

    for (int i = 0; i < size; ++i) {
        printf("%d\n", array[i]);
    }

    return 0;
}
