# evaluate-expression

- defautlt behavior:
executes expressions given as string (like "(2 + 3) ^ 5") and prints the result in stdout.
result is calculated using the shunting yard algorithm then computing the rpn expression. 

- -rpn option:
takes rpn expressions (like "1 1 +") and prints the result in stdout.

# error handling
inexpected inputs willl result in error code 1 (unaccepted ascii character) or 2 (syntax error).
