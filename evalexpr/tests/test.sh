#!/bin/sh

check() {
    local input="$1"
    local expected="$2"

    echo -n "$input" | ./evalexpr

    if [ $? -eq "$expected" ]; then
        echo OK
    else
        echo KO
    fi
}
check_out_rpn() {
    local input="$1"
    local expected="$2"

    n=$(echo -n "$input" | ./evalexpr -rpn)

    if [ $n -eq "$expected" ]; then
        echo OK
    else
        echo KO
    fi
}
check_out() {
    local input="$1"
    local expected="$2"

    n=$(echo -n "$input" | ./evalexpr)

    if [ $n -eq "$expected" ]; then
        echo OK
    else
        echo KO
    fi
}

check_rpn() {
    local input="$1"
    local expected="$2"
    echo -n "$input" | ./evalexpr -rpn

    if [ $? -eq "$expected" ]; then
        echo OK
    else
        echo KO
    fi
}

echo "defri" | ./evalexpr -a

if [ $? -eq 4 ]; then
    echo OK
else
    echo KO
fi

check_out "(--110 ++ 1) * 2 /1 % 220 - 2" 0
check_out_rpn " 1 1 1 + +" 3
check_out_rpn " 1 1 * 1 / 1 % 1 ^ 1 -" 0
check "111 + 1 *- 2 ^ 4 / (4 - 1) % 6" 0
#check "" 0
#check "a" 1
#check ")(" 2
#check "(" 2
#check "1 + 1 + 1 + 1()" 2
#check_rpn "1 1+" 0
#check_rpn "a" 1
#check "1/0" 3
#check "1%0" 3
#check "1^ -1" 3
