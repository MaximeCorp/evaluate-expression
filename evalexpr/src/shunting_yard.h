#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "compute_rpn.h"
#include "stack.h"

struct value *char_to_val(char c);

int pred(struct value *c);

int is_valid_shunting(char c);

int op_make(struct stack *op_stack, struct stack *output, struct value *op);

int shunting_yard(char *input, struct stack *output);

#endif /* ! SHUNTING_YARD_H */
