#include "compute_rpn.h"

struct dint dinit(int n)
{
    struct dint res = { .val = n, .err = 0 };

    return res;
}

static int my_pow(int a, int b)
{
    int res = 1;

    for (int i = 0; i < b / 2; i++)
    {
        res *= a;
    }

    res *= res;

    res *= (b % 2 == 0) ? 1 : a;

    return res;
}

static int is_valid(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 2;
    }

    char op[6] = { '+', '-', '*', '/', '%', '^' };

    for (size_t i = 0; i < 6; i++)
    {
        if (op[i] == c)
        {
            return 1;
        }
    }

    return 0;
}

struct value opc_to_val(char c)
{
    char op[6] = { '+', '-', '*', '/', '%', '^' };

    size_t i = 0;

    while (i < 6 && op[i] != c)
    {
        i++;
    }

    return value_init(TYPE_OP, i);
}

static void val_cmp(struct value *c, struct value *val1, struct value *val2,
                    struct value **r)
{
    if (!val1 || !val2)
    {
        free(*r);
        *r = NULL;
        return;
    }

    int resu;

    if (c->elt.operation == ADD)
    {
        **r = value_init(TYPE_INT, val2->elt.value + val1->elt.value);
    }
    else if (c->elt.operation == SUB)
    {
        **r = value_init(TYPE_INT, val2->elt.value - val1->elt.value);
    }
    else if (c->elt.operation == MULT)
    {
        **r = value_init(TYPE_INT, val2->elt.value * val1->elt.value);
    }
    else if (c->elt.operation == DIV)
    {
        if (!val1->elt.value)
        {
            c->token = TYPE_INT;
        }
        else
        {
            **r = value_init(TYPE_INT, val2->elt.value / val1->elt.value);
        }
    }
    else if (c->elt.operation == MOD)
    {
        if (!val1->elt.value)
        {
            c->token = TYPE_INT;
        }
        else
        {
            **r = value_init(TYPE_INT, val2->elt.value % val1->elt.value);
        }
    }
    else if (c->elt.operation == POW)
    {
        if (val1->elt.value < 0)
        {
            c->token = TYPE_INT;
        }
        else
        {
            resu = my_pow(val2->elt.value, val1->elt.value);
            **r = value_init(TYPE_INT, resu);
        }
    }
    else
    {
        free(*r);
        *r = NULL;
    }
}

static struct dint parse_op(struct value *op, struct stack *stack,
                            struct dint res)
{
    struct value *val1 = stack_pop(stack);
    struct value *val2 = stack_pop(stack);
    struct value *val = malloc(sizeof(struct value));

    val_cmp(op, val1, val2, &val);

    if (!val || op->token == TYPE_INT)
    {
        res.err = op->token == TYPE_OP ? 2 : 3;
        free(val);
        free(val1);
        free(val2);
        return res;
    }

    stack_push(stack, val);

    free(val);
    free(val1);
    free(val2);

    return res;
}

struct value *atoi_val(char *expression, size_t *i)
{
    size_t j = 0;

    int is_digit = is_valid(expression[*i + j]) == 2;
    while (expression[*i + j] && is_digit)
    {
        is_digit = is_valid(expression[++j]) == 2;
    }

    char *oui = malloc(sizeof(char) * (j + 1));

    for (size_t k = 0; k < j; k++)
    {
        oui[k] = expression[*i + k];
    }
    oui[j] = '\0';
    *i += j - 1;

    struct value *val = malloc(sizeof(struct value));

    int n = atoi(oui);

    val->elt.value = n;
    val->token = TYPE_INT;

    free(oui);

    return val;
}

struct stack *str_to_stack(char *expr)
{
    struct stack *stack = stack_init();

    while (*expr)
    {
        if (*expr != ' ')
        {
            int valid = is_valid(*expr);

            if (!valid)
            {
                return NULL;
            }

            if (valid == 2)
            {
                size_t i = 0;
                struct value *val = atoi_val(expr, &i);

                expr += i;
                stack_push(stack, val);
                free(val);
            }
            else
            {
                struct value *op = char_to_val(*expr);
                stack_push(stack, op);
                free(op);
            }
        }
        expr++;
    }

    return stack;
}

struct dint compute_rpn(struct stack *expr)
{
    struct stack *stack = stack_init();

    struct dint res = dinit(0);

    while (!stack_is_empty(expr))
    {
        struct value *cur = stack_pop_last(expr);
        if (cur->token == TYPE_OP)
        {
            res = parse_op(cur, stack, res);

            if (res.err != 0)
            {
                free(cur);
                break;
            }
        }
        else
        {
            stack_push(stack, cur);
        }
        free(cur);
    }
    if (res.err == 0)
    {
        struct value *last = stack_pop(stack);

        if (last->token != TYPE_INT || !stack->next)
        {
            res.val = last->elt.value;
        }
        else
        {
            res.err = 2;
        }

        free(last);
    }

    stack_clear(stack);

    free(stack);

    return res;
}
