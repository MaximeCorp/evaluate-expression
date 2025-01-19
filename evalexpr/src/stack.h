#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

enum operations
{
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    POW,
    L_PAR,
    R_PAR
};

enum val_type
{
    TYPE_OP,
    TYPE_INT
};

union element
{
    int value;
    enum operations operation;
};

struct value
{
    enum val_type token;
    union element elt;
};

struct stack
{
    struct value data;
    struct stack *next;
};

struct value value_init(enum val_type token, int n);

struct stack *stack_init(void);

void stack_push(struct stack *stack, struct value *ele);

struct value *stack_pop(struct stack *stack);

struct value *stack_pop_last(struct stack *stack);

int stack_is_empty(struct stack *stack);

struct value *stack_head(struct stack *stack);

void stack_clear(struct stack *);

#endif /* ! STACK_H */
