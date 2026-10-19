/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// >------------------------------------------------------------- operator stack

#define OP_STACK_OK            (1)
#define OP_STACK_ERR           (0)
#define OP_STACK_BASE_CAPACITY (2)
#define OP_STACK_GROWTH_FACTOR (2)
#define OP_STACK_MAX_SIZE      (SIZE_MAX)

typedef struct {
    size_t size;
    size_t capacity;
    char* data;
} op_stack_t;

op_stack_t op_stack_make(void) { return (op_stack_t){0}; }

void op_stack_destroy(op_stack_t* stack) {
    if (!stack) {
        return;
    }

    free((void*)stack->data);
    *stack = (op_stack_t){0};
}

int op_stack_is_empty(const op_stack_t* stack) {
    if (!stack) {
        return 1;
    }

    return stack->size == 0;
}

int op_stack_ensure_capacity(op_stack_t* stack, size_t count) {
    if (!stack) {
        return OP_STACK_ERR;
    }

    if (count <= stack->capacity - stack->size) {
        return OP_STACK_OK;
    }

    size_t max_capacity = OP_STACK_MAX_SIZE / sizeof(char);
    if (count > max_capacity - stack->size) {
        return OP_STACK_ERR;
    }

    size_t needed       = stack->size + count;
    size_t new_capacity = stack->capacity
                              ? stack->capacity * OP_STACK_GROWTH_FACTOR
                              : OP_STACK_BASE_CAPACITY;

    if (stack->capacity > max_capacity / OP_STACK_GROWTH_FACTOR) {
        new_capacity = max_capacity;
    }

    if (new_capacity < needed) {
        new_capacity = needed;
    }

    char* new_data = realloc((void*)stack->data, new_capacity * sizeof(char));
    if (!new_data) {
        return OP_STACK_ERR;
    }

    stack->data     = new_data;
    stack->capacity = new_capacity;
    return OP_STACK_OK;
}

void op_stack_push_unchecked(op_stack_t* stack, char op) {
    assert(stack && stack->size < stack->capacity);

    stack->data[stack->size++] = op;
}

int op_stack_push(op_stack_t* stack, char op) {
    if (!stack || !op_stack_ensure_capacity(stack, 1)) {
        return OP_STACK_ERR;
    }

    op_stack_push_unchecked(stack, op);
    return OP_STACK_OK;
}

char op_stack_peek_unchecked(const op_stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[stack->size - 1];
}

int op_stack_peek(const op_stack_t* stack, char* out_op) {
    if (!stack || !stack->size) {
        *out_op = '\0';
        return OP_STACK_ERR;
    }

    *out_op = op_stack_peek_unchecked(stack);
    return OP_STACK_OK;
}

char op_stack_pop_unchecked(op_stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[--stack->size];
}

int op_stack_pop(op_stack_t* stack, char* out_op) {
    if (!stack || !stack->size) {
        *out_op = '\0';
        return OP_STACK_ERR;
    }

    *out_op = op_stack_pop_unchecked(stack);
    return OP_STACK_OK;
}

void op_stack_print(const op_stack_t* stack) {
    if (!stack) {
        return;
    }

    printf("operator stack: [");
    for (size_t i = 0; i < stack->size; ++i) {
        if (i) {
            printf(", ");
        }

        printf("%c", stack->data[i]);
    }
    printf("] (size: %zu)\n", stack->size);
}

// >----------------------------------------------------- number reference stack

#define NUM_STACK_OK            (1)
#define NUM_STACK_ERR           (0)
#define NUM_STACK_BASE_CAPACITY (2)
#define NUM_STACK_GROWTH_FACTOR (2)
#define NUM_STACK_MAX_SIZE      (SIZE_MAX)

typedef struct {
    size_t pos;
    size_t len;
} num_ref_t;

num_ref_t num_ref_zero(void) { return (num_ref_t){0}; }

void num_ref_print(const num_ref_t* ref) {
    printf("{pos: %zu, len: %zu}", ref->pos, ref->len);
}

typedef struct {
    size_t size;
    size_t capacity;
    num_ref_t* data;
} num_stack_t;

num_stack_t num_stack_make(void) { return (num_stack_t){0}; }

void num_stack_destroy(num_stack_t* stack) {
    if (!stack) {
        return;
    }

    free((void*)stack->data);
    *stack = (num_stack_t){0};
}

int num_stack_is_empty(const num_stack_t* stack) {
    if (!stack) {
        return 1;
    }

    return stack->size == 0;
}

int num_stack_ensure_capacity(num_stack_t* stack, size_t count) {
    if (!stack) {
        return NUM_STACK_ERR;
    }

    if (count <= stack->capacity - stack->size) {
        return NUM_STACK_OK;
    }

    size_t max_capacity = NUM_STACK_MAX_SIZE / sizeof(num_ref_t);
    if (count > max_capacity - stack->size) {
        return NUM_STACK_ERR;
    }

    size_t needed       = stack->size + count;
    size_t new_capacity = stack->capacity
                              ? stack->capacity * NUM_STACK_GROWTH_FACTOR
                              : NUM_STACK_BASE_CAPACITY;

    if (stack->capacity > max_capacity / NUM_STACK_GROWTH_FACTOR) {
        new_capacity = max_capacity;
    }

    if (new_capacity < needed) {
        new_capacity = needed;
    }

    num_ref_t* new_data =
        realloc((void*)stack->data, new_capacity * sizeof(num_ref_t));
    if (!new_data) {
        return NUM_STACK_ERR;
    }

    stack->data     = new_data;
    stack->capacity = new_capacity;
    return NUM_STACK_OK;
}

void num_stack_push_unchecked(num_stack_t* stack, num_ref_t ref) {
    assert(stack && stack->size < stack->capacity);

    stack->data[stack->size++] = ref;
}

int num_stack_push(num_stack_t* stack, num_ref_t ref) {
    if (!stack || !num_stack_ensure_capacity(stack, 1)) {
        return NUM_STACK_ERR;
    }

    num_stack_push_unchecked(stack, ref);
    return NUM_STACK_OK;
}

num_ref_t num_stack_peek_unchecked(const num_stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[stack->size - 1];
}

int num_stack_peek(const num_stack_t* stack, num_ref_t* out_ref) {
    if (!stack || !stack->size) {
        *out_ref = num_ref_zero();
        return NUM_STACK_ERR;
    }

    *out_ref = num_stack_peek_unchecked(stack);
    return NUM_STACK_OK;
}

num_ref_t num_stack_pop_unchecked(num_stack_t* stack) {
    assert(stack && stack->size);

    return stack->data[--stack->size];
}

int num_stack_pop(num_stack_t* stack, num_ref_t* out_ref) {
    if (!stack || !stack->size) {
        *out_ref = num_ref_zero();
        return NUM_STACK_ERR;
    }

    *out_ref = num_stack_pop_unchecked(stack);
    return NUM_STACK_OK;
}

void num_stack_print(const num_stack_t* stack) {
    if (!stack) {
        return;
    }

    printf("number reference stack: [");
    for (size_t i = 0; i < stack->size; ++i) {
        if (i) {
            printf(", ");
        }

        num_ref_print(&stack->data[i]);
    }
    printf("] (size: %zu)\n", stack->size);
}

// >------------------------------------------------------------- operator utils

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

    if (!op_stack_ensure_capacity(&stack, strlen(str))) {
        out[0] = '\0';
        return 0;
    }

    while (str[str_pos] != '\0') {
        char ch = str[str_pos];

        if (isdigit((unsigned char)ch)) {
            out[out_pos++] = ch;
        } else if (op_is_valid(ch)) {
            out[out_pos++] = ' ';

            while (!op_stack_is_empty(&stack)) {
                char top  = op_stack_peek_unchecked(&stack);
                int p_top = op_precedence(top);
                int p_ch  = op_precedence(ch);

                if (top == '(' || p_top < p_ch ||
                    (p_top == p_ch && ch == '^')) {
                    break;
                }

                op_stack_pop_unchecked(&stack);
                out[out_pos++] = top;
                out[out_pos++] = ' ';
            }

            op_stack_push_unchecked(&stack, ch);
        } else if (ch == '(') {
            op_stack_push_unchecked(&stack, '(');
        } else if (ch == ')') {
            char top = '\0';

            while (op_stack_pop(&stack, &top) && top != '(') {
                out[out_pos++] = ' ';
                out[out_pos++] = top;
            }
        }

        ++str_pos;
    }

    if (!op_stack_is_empty(&stack)) {
        out[out_pos++] = ' ';
    }

    while (!op_stack_is_empty(&stack)) {
        out[out_pos++] = op_stack_pop_unchecked(&stack);
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

    if (!num_stack_ensure_capacity(&stack, strlen(str))) {
        out[0] = '\0';
        return 0;
    }

    while (str[str_pos] != '\0') {
        char ch = str[str_pos];

        while (isspace((unsigned char)ch)) {
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
            num_stack_push_unchecked(&stack, num);
        } else if (op_is_valid(ch)) {
            num_ref_t right = num_ref_zero();
            num_ref_t left  = num_ref_zero();

            num_stack_pop(&stack, &right);
            num_stack_pop(&stack, &left);

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

            num_stack_push_unchecked(&stack, expr);
            ++str_pos;
        }
    }

    out[out_pos] = '\0';
    num_stack_destroy(&stack);

    return out_pos;
}

// >----------------------------------------------------------------------- main

void test_op_stack(void) {
    printf("---- operator stack\n");

    op_stack_t stack = op_stack_make();
    char op          = '\0';

    op_stack_push(&stack, '(');
    op_stack_push(&stack, '+');
    op_stack_push(&stack, '*');
    op_stack_print(&stack);

    op_stack_pop(&stack, &op);
    printf("pop: %c\n", op);

    op_stack_print(&stack);
    op_stack_destroy(&stack);
    printf("\n");
}

void test_num_stack(void) {
    printf("---- number reference stack\n");

    num_stack_t stack = num_stack_make();
    num_ref_t ref     = num_ref_zero();

    num_stack_push(&stack, (num_ref_t){.pos = 0, .len = 2});
    num_stack_push(&stack, (num_ref_t){.pos = 3, .len = 1});
    num_stack_print(&stack);

    num_stack_pop(&stack, &ref);
    printf("pop: ");
    num_ref_print(&ref);
    printf("\n");

    num_stack_print(&stack);
    num_stack_destroy(&stack);
    printf("\n");
}

void test_conversion(void) {
    printf("---- conversion\n");

    const char str[] = "12 + 2 + 3 * (24 * (2 - 305)) ^ 2";
    char out_postfix[sizeof(str) * 2 + 1];
    char out_infix[sizeof(str) * 2 + 1];

    from_infix_to_postfix(str, out_postfix);
    from_postfix_to_infix(out_postfix, out_infix);

    printf("original: %s\n", str);
    printf("postfix: %s\n", out_postfix);
    printf("infix: %s\n", out_infix);
    printf("\n");
}

int main(void) {
    test_op_stack();
    test_num_stack();
    test_conversion();

    return 0;
}
