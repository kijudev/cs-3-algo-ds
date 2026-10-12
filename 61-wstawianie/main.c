/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.12
 */

#include <stddef.h>
#include <stdio.h>

void insertion_sort(int array[], int size) {
    for (int i = 1; i < size; ++i) {
        int value = array[i];
        int j     = i - 1;

        while (j >= 0 && array[j] > value) {
            array[j + 1] = array[j];
            --j;
        }

        array[j + 1] = value;
    }
}

int main(void) {
    int array[] = {6, 2, 7, 3, 5, 4, 1};
    int size    = sizeof(array) / sizeof(int);

    insertion_sort(array, size);

    for (int i = 0; i < size; ++i) {
        printf("%d\n", array[i]);
    }

    return 0;
}
