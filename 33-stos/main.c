/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.08
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// >----------------------------------------------------------------------- item

typedef struct {
    int value;
} item_t;

item_t item_make(int value) { return (item_t){value}; }
item_t item_zero(void) { return (item_t){0}; }

int item_eq(const item_t* a, const item_t* b) { return a->value == b->value; }

void item_print(const item_t* item) { printf("%d", item->value); }

// >---------------------------------------------------------------------- stack

#define STACK_OK            (1)
#define STACK_ERR           (0)
#define STACK_BASE_CAPACITY (2)
#define STACK_GROWTH_FACTOR (2)
#define STACK_MAX_SIZE      (SIZE_MAX)

typedef struct {
    size_t size;
    size_t capacity;
    item_t* data;
} stack_t;

stack_t stack_make(void) { return (stack_t){0}; }

void stack_destroy(stack_t* stack) {
    if (!stack) {
        return;
    }

    free((void*)stack->data);
    *stack = (stack_t){0};
}

int stack_is_full(const stack_t* stack) {
    if (!stack) {
        return 0;
    }

    return stack->size == stack->capacity;
}

int stack_is_empty(const stack_t* stack) {
    if (!stack) {
        return 1;
    }

    return stack->size == 0;
}

size_t stack_size(const stack_t* stack) {
    if (!stack) {
        return 0;
    }

    return stack->size;
}

int stack_ensure_capacity(stack_t* stack, size_t count) {
    if (!stack) {
        return STACK_ERR;
    }

    if (count <= stack->capacity - stack->size) {
        return STACK_OK;
    }

    size_t max_capacity = STACK_MAX_SIZE / sizeof(item_t);
    if (count > max_capacity - stack->size) {
        return STACK_ERR;
    }

    size_t needed       = stack->size + count;
    size_t new_capacity = stack->capacity
                              ? stack->capacity * STACK_GROWTH_FACTOR
                              : STACK_BASE_CAPACITY;

    if (stack->capacity > max_capacity / STACK_GROWTH_FACTOR) {
        new_capacity = max_capacity;
    }

    if (new_capacity < needed) {
        new_capacity = needed;
    }

    item_t* new_data =
        realloc((void*)stack->data, new_capacity * sizeof(item_t));
    if (!new_data) {
        return STACK_ERR;
    }

    stack->data     = new_data;
    stack->capacity = new_capacity;
    return STACK_OK;
}

void stack_push_unchecked(stack_t* stack, item_t item) {
    assert(stack && stack->size < stack->capacity);

    stack->data[stack->size++] = item;
}

int stack_push(stack_t* stack, item_t item) {
    if (!stack || !stack_ensure_capacity(stack, 1)) {
        return STACK_ERR;
    }

    stack_push_unchecked(stack, item);
    return STACK_OK;
}

item_t stack_peek_unchecked(const stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[stack->size - 1];
}

int stack_peek(const stack_t* stack, item_t* out_item) {
    if (!stack || !stack->size) {
        *out_item = item_zero();
        return STACK_ERR;
    }

    *out_item = stack_peek_unchecked(stack);
    return STACK_OK;
}

item_t stack_pop_unchecked(stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[--stack->size];
}

int stack_pop(stack_t* stack, item_t* out_item) {
    if (!stack || !stack->size) {
        *out_item = item_zero();
        return STACK_ERR;
    }

    *out_item = stack_pop_unchecked(stack);
    return STACK_OK;
}

void stack_print(const stack_t* stack) {
    if (!stack) {
        return;
    }

    printf("stack: [");
    for (size_t i = 0; i < stack->size; ++i) {
        if (i) {
            printf(", ");
        }

        item_print(&stack->data[i]);
    }
    printf("] (size: %zu)\n", stack->size);
}

// >----------------------------------------------------------------------- main

void test_push_pop(void) {
    printf("---- push pop\n");

    stack_t stack = stack_make();
    item_t item   = item_zero();

    stack_push(&stack, item_make(10));
    stack_push(&stack, item_make(20));
    stack_push(&stack, item_make(30));
    stack_print(&stack);

    stack_peek(&stack, &item);
    printf("peek: ");
    item_print(&item);
    printf("\n");

    stack_pop(&stack, &item);
    printf("pop: ");
    item_print(&item);
    printf("\n");

    stack_print(&stack);
    stack_destroy(&stack);
    printf("\n");
}

void test_empty(void) {
    printf("---- empty\n");

    stack_t stack = stack_make();
    item_t item   = item_zero();

    stack_print(&stack);
    printf("peek: %s\n", stack_peek(&stack, &item) ? "ok" : "err");
    printf("pop: %s\n", stack_pop(&stack, &item) ? "ok" : "err");

    stack_destroy(&stack);
    printf("\n");
}

void test_unchecked(void) {
    printf("---- unchecked\n");

    stack_t stack = stack_make();
    stack_push(&stack, item_make(0));

    if (stack_ensure_capacity(&stack, 3)) {
        stack_push_unchecked(&stack, item_make(1));
        stack_push_unchecked(&stack, item_make(2));
        stack_push_unchecked(&stack, item_make(3));
    }
    stack_print(&stack);

    item_t item = stack_pop_unchecked(&stack);
    printf("pop: ");
    item_print(&item);
    printf("\n");

    stack_print(&stack);
    stack_destroy(&stack);
    printf("\n");
}

int main(void) {
    test_push_pop();
    test_empty();
    test_unchecked();

    return 0;
}
