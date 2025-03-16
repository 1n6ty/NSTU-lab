#!/bin/bash

PRECISION=$3; # Set precision of float math from the third argument

if [ $(echo "${1%.*} < $1" | bc) = "1" ] || [ $(echo "${2%.*} < $2" | bc) = "1" ] || [ $(expr $1 % $2) != "0" ]; then # Check whether the first argument type is float or the first argument type is float or mod is not equal to 0
    printf "%.${PRECISION}f" $(echo "$1 / $2" | bc -l); # Float math
else
    expr $1 / $2 | tr -d '\n'; # Integer math
fi
