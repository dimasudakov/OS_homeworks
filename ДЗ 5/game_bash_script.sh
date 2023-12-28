#!/bin/bash

RED='\e[31m'
GREEN='\e[32m'
RESET='\e[0m'

hit=0
miss=0
declare -a numbers

while :
do
    echo "Step: ${#numbers[@]}"
    read -p "Please enter number from 0 to 9 (q - quit): " input

    case "$input" in
        [0-9])
            if [ "${#input}" -eq 1 ]; then
                random_number=$(( RANDOM % 10 ))
                if [ "$input" -eq "$random_number" ]; then
                    echo -e "Hit! My number: ${random_number}"
                    hit=$((hit + 1))
                    numbers+=("${GREEN}${random_number}${RESET}")
                else
                    echo -e "Miss! My number: ${random_number}"
                    miss=$((miss + 1))
                    numbers+=("${RED}${random_number}${RESET}")
                fi

                total=$((hit + miss))
                hit_percent=$((hit * 100 / total))
                miss_percent=$((100 - hit_percent))

                echo "Hit: ${hit_percent}%" "Miss: ${miss_percent}%"

                if [ "${#numbers[@]}" -lt 10 ]; then
                    echo -e "Numbers: ${numbers[@]: -${#numbers[@]}}"
                else
                    echo -e "Numbers: ${numbers[@]: -10}"
                fi

            else
                echo "Not valid input"
                echo "Please repeat"
            fi
        ;;
        q)
            echo "Bye"
            echo "Exit..."
            exit 0
        ;;
        *)
            echo "Not valid input"
            echo "Please repeat"
        ;;
    esac
done
