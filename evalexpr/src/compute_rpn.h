#ifndef COMPUTE_RPN_H
#define COMPUTE_RPN_H

#include <string.h>

#include "shunting_yard.h"
#include "stack.h"

struct dint
{
    int val;
    int err;
};

struct dint dinit(int n);

struct value opc_to_val(char c);

struct value *atoi_val(char *expression, size_t *i);

struct stack *str_to_stack(char *expr);

struct dint compute_rpn(struct stack *expr);

#endif /* ! COMPUTE_RPN_H */
