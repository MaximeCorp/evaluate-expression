#include "compute_rpn.h"
#include "shunting_yard.h"

static int is_rpn(int argc, char **argv)
{
    int is_rpn = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-rpn"))
        {
            is_rpn = 1;
        }
        else
        {
            return 4;
        }
    }

    return is_rpn;
}

int main(int argc, char **argv)
{
    char b[1024];
    size_t bytes = fread(b, 1, sizeof(b) - 1, stdin);

    b[bytes] = '\0';

    size_t n = strlen(b);

    if (!n)
    {
        return 0;
    }

    int rpn = is_rpn(argc, argv);

    if (rpn == 4)
    {
        return 4;
    }

    struct stack *output = stack_init();

    if (!rpn)
    {
        int err = shunting_yard(b, output);

        if (err != 0)
        {
            stack_clear(output);
            free(output);
            return err;
        }
    }
    else
    {
        free(output);
        output = str_to_stack(b);

        if (!output)
        {
            stack_clear(output);
            free(output);
            return 1;
        }
    }

    struct dint res = compute_rpn(output);

    if (!res.err)
    {
        printf("%d\n", res.val);
    }

    stack_clear(output);
    free(output);

    return res.err;
}
