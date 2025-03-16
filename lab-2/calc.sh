#!/bin/bash

PRECISION=2; # Precision of float number

echo `date`; # Prints current date
echo "PMI-33, Team - 1"; # Developers' info 

printf "Calculator:\n";

while [ true ]; do # Menu cycle
    while [ true ]; do
        printf "Input first number (or type 'quit' to quit): ";
        read FST;
        if [ $FST = "quit" ]; then # Command to quit the program
            exit 0;
        fi
        if ! [[ $FST =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]; then # Regular expression to check whether FST variable is a number
            echo "Incorrect type, try again";
        else
            break;
        fi
    done

    while [ true ]; do
        printf "Input operator {'+', '-', '\*', '/'}: ";
        read OP;
        if ! [[ $OP =~ [+-'\*'/] ]]; then # Regular expression to check whether OP variable is a valid operator
            echo "Incorrect type, try smth from {'+', '-', '\*', '/'}";
        else
            break;
        fi
    done

    while [ true ]; do
        printf "Input second number: ";
        read SEC;
        if ! [[ $SEC =~ ^[+-]?[0-9]+([.][0-9]+)?$ ]]; then # Regular expression to check whether SEC variable is a number
            echo "Incorrect type, try again";
        else
            break;
        fi
    done

    case $OP in # Switch/case to run specified operator
        "+")
            ./operators/add.sh $FST $SEC $PRECISION ;;

        "-")
            ./operators/sub.sh $FST $SEC $PRECISION ;;
        
        "*")
            ./operators/multiply.sh $FST $SEC $PRECISION ;;

        "/")
            ./operators/divide.sh $FST $SEC $PRECISION ;;
    esac
    echo;
done
