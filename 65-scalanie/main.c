/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 */

#include <stddef.h>
#include <stdio.h>

void merge_sort(int array[], int size) {}

int main(void) {
    int array[] = {6, 2, 7, 3, 5, 4, 1};
    int size    = sizeof(array) / sizeof(int);

    merge_sort(array, size);

    for (int i = 0; i < size; ++i) {
        printf("%d\n", array[i]);
    }

    return 0;
}
