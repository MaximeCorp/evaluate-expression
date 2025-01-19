#include "stack.h"

struct value value_init(enum val_type token, int n)
{
    union element elt;
    if (token == TYPE_INT)
    {
        elt.value = n;
    }
    else
    {
        elt.operation = n;
    }

    struct value res = { .token = token, .elt = elt };
    return res;
}

struct stack *stack_init(void)
{
    struct stack *res = calloc(1, sizeof(struct stack));

    return res;
}

void stack_push(struct stack *stack, struct value *ele)
{
    if (!ele)
    {
        return;
    }

    struct stack *stalck = calloc(1, sizeof(struct stack));

    stalck->data = *ele;

    if (stack->next)
    {
        stalck->next = stack->next;
    }

    stack->next = stalck;
}

struct value *stack_pop(struct stack *stack)
{
    if (!stack || !stack->next)
    {
        return NULL;
    }

    struct value *res = calloc(1, sizeof(struct value));

    *res = stack->next->data;

    struct stack *popped = stack->next;

    stack->next = popped->next;

    free(popped);

    return res;
}

struct value *stack_pop_last(struct stack *stack)
{
    if (!stack || !stack->next)
    {
        return NULL;
    }

    struct stack *prev = stack;
    stack = stack->next;

    while (stack->next)
    {
        stack = stack->next;
        prev = prev->next;
    }

    struct value *result = malloc(sizeof(struct value));

    result->token = stack->data.token;
    result->elt = stack->data.elt;

    prev->next = NULL;

    free(stack);

    return result;
}

void stack_clear(struct stack *stack)
{
    while (stack->next)
    {
        free(stack_pop(stack));
    }
}

int stack_is_empty(struct stack *stack)
{
    return !stack || !stack->next;
}

struct value *stack_head(struct stack *stack)
{
    if (!stack || !stack->next)
    {
        return NULL;
    }

    return &(stack->next->data);
}
