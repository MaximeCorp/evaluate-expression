#include "shunting_yard.h"

struct value *char_to_val(char c)
{
    char op[8] = { '+', '-', '*', '/', '%', '^', '(', ')' };
    int i = 0;

    while (c != op[i])
    {
        i++;
    }

    if (i == 8)
    {
        return NULL;
    }

    enum operations en[8] = { ADD, SUB, MULT, DIV, MOD, POW, L_PAR, R_PAR };

    struct value *res = malloc(sizeof(struct value));

    res->token = TYPE_OP;
    res->elt.operation = en[i];

    return res;
}

int pred(struct value *c)
{
    int a[8] = { 1, 1, 2, 2, 2, 3, 4, 4 };

    return a[c->elt.operation];
}

int is_valid_shunting(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 2;
    }

    char op[8] = { '+', '-', '*', '/', '%', '^', '(', ')' };

    for (size_t i = 0; i < 8; i++)
    {
        if (op[i] == c)
        {
            return 1;
        }
    }

    return 0;
}

static void pop_push(struct stack *op_stack, struct stack *output)
{
    struct value *v = stack_pop(op_stack);
    stack_push(output, v);
    free(v);
}

int op_make(struct stack *op_stack, struct stack *output, struct value *op)
{
    int pre = pred(op);

    if (pre < 4)
    {
        struct value *o2 = stack_head(op_stack);
        if (o2)
        {
            int n = o2->elt.operation;

            int left1 = pre < 3 && pred(o2) == pre;

            while ((o2 && n != L_PAR && pred(o2) > pre) || left1)
            {
                pop_push(op_stack, output);
                o2 = stack_head(op_stack);
                left1 = 0;
                if (o2)
                {
                    n = o2->elt.operation;
                    left1 = pre < 3 && pred(o2) == pre;
                }
            }
        }

        stack_push(op_stack, op);
    }
    else if (op->elt.operation == L_PAR)
    {
        stack_push(op_stack, op);
    }
    else
    {
        pop_push(op_stack, output);
        struct value *o2 = stack_head(op_stack);
        int n = o2->elt.operation;
        while (n != L_PAR)
        {
            struct value *o2 = stack_head(op_stack);
            o2 = stack_head(op_stack);
            if (!o2)
            {
                return 2;
            }
            n = o2->elt.value;
        }

        free(stack_pop(op_stack));
    }

    free(op);
    return 0;
}

struct value *make_val(int n)
{
    struct value *val = malloc(sizeof(struct value));
    val->token = TYPE_INT;
    val->elt.value = n;

    return val;
}

int finish_stack(struct stack *op_stack, struct stack *output, int res)
{
    while (!stack_is_empty(op_stack))
    {
        struct value *oper = stack_pop(op_stack);

        if (oper->elt.operation == L_PAR || oper->elt.operation == R_PAR)
        {
            free(oper);
            stack_clear(op_stack);
            free(op_stack);
            return 2;
        }

        stack_push(output, oper);
        free(oper);
    }

    return res;
}

void manage_num(struct stack *output, char **input)
{
    size_t i = 0;

    struct value *val = atoi_val(*input, &i);

    stack_push(output, val);

    free(val);

    *input += i;
}

int shunting_yard(char *input, struct stack *output)
{
    int res = 0;
    struct stack *op_stack = stack_init();

    int last = 1;

    while (*input)
    {
        if (*input != ' ')
        {
            int valid = is_valid_shunting(*input);

            if (!valid)
            {
                return 1;
            }

            if (valid == 2)
            {
                manage_num(output, &input);
                last = 0;
            }
            else
            {
                struct value *op = char_to_val(*input);

                if (last && (*input == '-' || *input == '+'))
                {
                    if (*input == '-')
                    {
                        struct value *val = make_val(0);
                        stack_push(output, val);
                        stack_push(op_stack, op);
                        free(val);
                    }
                    free(op);
                }
                else
                {
                    res = op_make(op_stack, output, op);

                    if (res)
                    {
                        break;
                    }
                }

                last = 1;
            }
        }
        input++;
    }

    int x = finish_stack(op_stack, output, res);
    stack_clear(op_stack);
    free(op_stack);
    return x;
}
