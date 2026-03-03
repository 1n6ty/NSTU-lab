#!/bin/bash

if [ -z "$1" ]; then
    echo "Использование: $0 <имя_каталога>"
    exit 1
fi

if ! cd "$1" 2>/dev/null; then
    echo "Ошибка: не удалось перейти в каталог $1"
    exit 1
fi

shopt -s dotglob

while true; do
    echo "каталог $(pwd)"
    
    for dir in */ ; do
        if [ -d "$dir" ] && [ "$dir" != "./" ] && [ "$dir" != "../" ]; then
            echo -e "\tкаталог ${dir%/}"
        fi
    done

    echo -e "\n...............\n"

    if [ "$(pwd)" == "/" ]; then
        break
    fi

    if ! cd .. ; then
        echo "Ошибка перехода в родительский каталог"
        break
    fi
done