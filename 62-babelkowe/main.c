/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.13
 */

#include <stddef.h>
#include <stdio.h>

void quick_sort(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp     = array[j];
                array[j]     = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    int array[] = {6, 2, 7, 3, 5, 4, 1};
    int size    = sizeof(array) / sizeof(int);

    quick_sort(array, size);

    for (int i = 0; i < size; ++i) {
        printf("%d\n", array[i]);
    }

    return 0;
}
