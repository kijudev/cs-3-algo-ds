/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <assert.h>
#include <bits/types/stack_t.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// >-------------------------------------------------------------- operand stack

typedef struct {
    size_t size;
    size_t capacity;
    char* data;
} op_stack_t;

op_stack_t op_stack_make(void) { return (op_stack_t){0}; }

void op_stack_push(op_stack_t* stack, char op) {
    assert(stack);

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
    assert(stack);

    if (!stack->size) {
        return '\0';
    }

    return stack->data[--stack->size];
}

char op_stack_peek(const op_stack_t* stack) {
    assert(stack);

    if (!stack->size) {
        return '\0';
    }

    return stack->data[stack->size - 1];
}

void op_stack_destroy(op_stack_t* stack) {
    if (!stack) {
        return;
    }

    free((void*)stack->data);
    *stack = (op_stack_t){0};
}

// >----------------------------------------------------- number reference stack

typedef struct {
    size_t pos;
    size_t len;
} num_ref_t;

typedef struct {
    size_t size;
    size_t capacity;
    num_ref_t* data;
} num_stack_t;

num_stack_t num_stack_make(void) { return (num_stack_t){0}; }

void num_stack_push(num_stack_t* stack, num_ref_t ref) {
    assert(stack);

    if (!stack->data) {
        stack->capacity = 2;
        stack->data     = malloc(2 * sizeof(num_ref_t));
    } else if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->data =
            realloc((void*)stack->data, stack->capacity * sizeof(num_ref_t));
    }

    stack->data[stack->size++] = ref;
}

num_ref_t num_stack_pop(num_stack_t* stack) {
    assert(stack);

    if (!stack->size) {
        return (num_ref_t){0};
    }

    return stack->data[--stack->size];
}

num_ref_t num_stack_peek(const num_stack_t* stack) {
    assert(stack);

    if (!stack->size) {
        return (num_ref_t){0};
    }

    return stack->data[stack->size - 1];
}

void num_stack_destroy(num_stack_t* stack) {
    if (!stack) {
        return;
    }

    free((void*)stack->data);
    *stack = (num_stack_t){0};
}

// >-------------------------------------------------------------- operand utlis

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

int op_is_valid(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// >----------------------------------------------------------------- algorithms

size_t from_infix_to_postfix(const char* str, char* out) {
    assert(str);
    assert(out);

    size_t str_pos   = 0;
    size_t out_pos   = 0;
    op_stack_t stack = op_stack_make();

    while (str[str_pos] != '\0') {
        char ch = str[str_pos];

        if (isdigit((unsigned char)ch)) {
            out[out_pos++] = ch;
        } else if (op_is_valid(ch)) {
            out[out_pos++] = ' ';

            while (stack.size > 0) {
                char top  = op_stack_peek(&stack);
                int p_top = op_precedence(top);
                int p_ch  = op_precedence(ch);

                if (top == '(' || p_top < p_ch ||
                    (p_top == p_ch && ch == '^')) {
                    break;
                }

                op_stack_pop(&stack);
                out[out_pos++] = top;
                out[out_pos++] = ' ';
            }

            op_stack_push(&stack, ch);
        } else if (ch == '(') {
            op_stack_push(&stack, '(');
        } else if (ch == ')') {
            char top = op_stack_pop(&stack);
            while (top != '(' && top != '\0') {
                out[out_pos++] = ' ';
                out[out_pos++] = top;
                top            = op_stack_pop(&stack);
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
    op_stack_destroy(&stack);

    return out_pos;
}

size_t from_postfix_to_infix(const char* str, char* out) {
    assert(str);
    assert(out);

    size_t str_pos    = 0;
    size_t out_pos    = 0;
    num_stack_t stack = num_stack_make();

    while (str[str_pos] != '\0') {
        char ch = str[str_pos];

        while (isspace(ch)) {
            ch = str[++str_pos];
        }

        if (isdigit((unsigned char)ch)) {
            num_ref_t num = {
                .pos = out_pos,
                .len = 0,
            };

            while (isdigit((unsigned char)ch)) {
                out[out_pos++] = ch;
                ch             = str[++str_pos];
            }

            num.len = out_pos - num.pos;
            num_stack_push(&stack, num);
        } else if (op_is_valid(ch)) {
            num_ref_t right = num_stack_pop(&stack);
            num_ref_t left  = num_stack_pop(&stack);

            memmove((void*)(out + right.pos + 2),
                    (const void*)(out + right.pos), right.len);

            out[right.pos + 1] = ch;

            memmove((void*)(out + left.pos + 1), (const void*)(out + left.pos),
                    left.len);
            out[left.pos] = '(';

            out_pos += 2;
            out[out_pos++] = ')';

            num_ref_t expr = {
                .pos = left.pos,
                .len = out_pos - left.pos,
            };

            num_stack_push(&stack, expr);
            ++str_pos;
        }
    }

    out[out_pos] = '\0';
    num_stack_destroy(&stack);

    return out_pos;
}

// >---------------------------------------------------------------- application

int main(void) {
    char str[]        = "12 + 2 + 3 * (24 * (2 - 305)) ^ 2";
    char* out_postfix = malloc(sizeof(str) * 2 + 1);
    char* out_infix   = malloc(sizeof(str) * 2 + 1);

    from_infix_to_postfix(str, out_postfix);
    from_postfix_to_infix(out_postfix, out_infix);

    printf("Original: %s\n", str);
    printf("Postfix: %s\n", out_postfix);
    printf("Infix: %s\n", out_infix);

    free((void*)out_postfix);
    free((void*)out_infix);

    return 0;
}
