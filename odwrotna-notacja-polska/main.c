#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
    char* data;
} op_stack_t;

op_stack_t make_op_stack(void) {
    op_stack_t stack = {
        .size     = 0,
        .capacity = 0,
        .data     = NULL,
    };

    return stack;
}

void op_stack_push(op_stack_t* stack, char op) {
    if (!stack->data) {
        stack->capacity = 2;
        stack->data     = malloc(2 * sizeof(char));
    } else if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->data =
            realloc((void*)stack->data, stack->capacity * sizeof(char));
    }

    stack->data[stack->size++] = op;
}

char op_stack_pop(op_stack_t* stack) {
    if (!stack->size) {
        return '\0';
    }

    return stack->data[--stack->size];
}

char op_stack_peek(const op_stack_t* stack) {
    if (!stack->size) {
        return '\0';
    }

    return stack->data[stack->size - 1];
}

void op_stack_free(op_stack_t* stack) {
    stack->size     = 0;
    stack->capacity = 0;
    free((void*)stack->data);
}

int op_precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }

    if (op == '*' || op == '/') {
        return 2;
    }

    if (op == '^') {
        return 3;
    }

    return 0;
}

int is_op(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

int main(void) {
    char str[]     = "1 * 2 + 3";
    char* out      = malloc(sizeof(str) * 2 + 1);
    size_t str_pos = 0;
    size_t out_pos = 0;

    op_stack_t stack = make_op_stack();

    while (str_pos < sizeof(str)) {
        char ch = str[str_pos];

        if (isdigit(ch)) {
            out[out_pos++] = ch;
        } else if (is_op(ch)) {
            out[out_pos++] = ' ';

            char top = op_stack_peek(&stack);
            if (op_precedence(top) < op_precedence(ch)) {
                op_stack_push(&stack, ch);
            } else if (top != '\0') {
                op_stack_pop(&stack);
                out[out_pos++] = top;
                out[out_pos++] = ' ';
                op_stack_push(&stack, ch);
            }
        }

        ++str_pos;
    }

    if (stack.size > 0) {
        out[out_pos++] = ' ';
    }

    while (stack.size > 0) {
        char top       = op_stack_pop(&stack);
        out[out_pos++] = top;
        out[out_pos++] = ' ';
    }

    out[out_pos] = '\0';

    op_stack_free(&stack);
    printf("%s", out);

    return 0;
}
