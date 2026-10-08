/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_OK  (1)
#define STACK_ERR (0)

typedef struct {
    int value;
} item_t;

item_t item_make(int value) { return (item_t){value}; }
item_t item_zero(void) { return (item_t){0}; }

typedef struct {
    size_t size;
    size_t capacity;
    item_t* data;
} stack_t;

stack_t stack_make(void) { return (stack_t){0}; }

void stack_free(stack_t* stack) {
    free((void*)stack->data);
    *stack = (stack_t){0};
}

int stack_is_full(const stack_t* stack) {
    return stack->size == stack->capacity;
}

int stack_is_empty(const stack_t* stack) { return stack->size == 0; }

size_t stack_size(const stack_t* stack) { return stack->size; }

int stack_push(stack_t* stack, item_t item) {
    if (stack_is_full(stack)) {
        size_t new_capacity = stack->capacity ? stack->capacity * 2 : 2;

        if (new_capacity < stack->capacity ||
            new_capacity > SIZE_MAX / sizeof(item_t)) {
            return STACK_ERR;
        }

        item_t* new_data = realloc(stack->data, new_capacity * sizeof(item_t));
        if (!new_data) {
            return STACK_ERR;
        }

        stack->data     = new_data;
        stack->capacity = new_capacity;
    }

    stack->data[stack->size++] = item;
    return STACK_OK;
}

int stack_peek(const stack_t* stack, item_t* out_item) {
    if (!stack->size) {
        *out_item = item_zero();
        return STACK_ERR;
    }

    *out_item = stack->data[stack->size - 1];
    return STACK_OK;
}

int stack_pop(stack_t* stack, item_t* out_item) {
    if (!stack->size) {
        *out_item = item_zero();
        return STACK_ERR;
    }

    *out_item = stack->data[--stack->size];
    return STACK_OK;
}

int main(void) {
    stack_t stack = stack_make();

    stack_push(&stack, item_make(10));
    stack_push(&stack, item_make(20));
    stack_push(&stack, item_make(30));
    stack_push(&stack, item_make(40));

    size_t size = stack_size(&stack);

    for (size_t i = 0; i < size + 2; ++i) {
        item_t item = item_zero();
        int ok      = stack_pop(&stack, &item);

        if (ok) {
            printf("item: %d\n", item.value);
        } else {
            printf("error while trying to pop an item of the stack\n");
        }
    }

    stack_free(&stack);
    return 0;
}
