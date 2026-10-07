/**
 * @author Jakub Kijek, Informatyka, sem. 3, gr. 2
 * @date 2026.10.07
 */

#include <assert.h>
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

/**
 * @brief Transforms a string of infix notation arithmetic operations to postfix
 * notation. Only none-negitve ints and basic operations are supported (+, -, *,
 * /, ^). parens are also supported.
 *
 * @param str Source string.
 * @param out Outut string.
 * @return Size of the output string.
 *
 * @pre `str` must be non-null.
 * @pre `in` must be non-null.
 * @pre The output's string size has to be at least two times greater than the
 * input one. This is a very conservative estimate, but the overhead is
 * neglegible.
 *
 * @note This function works purely on strings, it does not serialize the data
 * into some intermidate state. The input is not checked, thus has to be
 * well-formed.
 */
size_t from_infix_to_postfix(const char* str, char* out) {
    assert(str);
    assert(out);

    size_t str_pos   = 0;
    size_t out_pos   = 0;
    op_stack_t stack = make_op_stack();

    while (str[str_pos] != '\0') {
        char ch = str[str_pos];

        if (isdigit((unsigned char)ch)) {
            out[out_pos++] = ch;
        } else if (is_op(ch)) {
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

    op_stack_free(&stack);
    return out_pos;
}

int main(void) {
    char str[] = "1 + 2 + 3 * (2 * (2 + 3))";
    char* out  = malloc(sizeof(str) * 2 + 1);

    from_infix_to_postfix(str, out);
    printf("%s", out);

    free((void*)out);
    return 0;
}
