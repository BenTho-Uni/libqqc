#!/bin/bash

for file in "$@"; do
    printf $file 
    printf ","
    awk '{if(NR==7) print $0}' $file | sed 's/ //g' | tr -d '\n'
    printf ","
    grep -oP '(?<=threads:   )[0-9,]+' $file | tr -d '\n'
    printf ","
    grep -oP '(?<=processes: )[0-9,]+' $file | tr -d '\n'
    printf ","
# Grep all timings, put them in csv format
    while read -r line; do
        printf $line
        printf ","
    done < <(grep -oP '(?<=Wall: )[0-9]+' $file)
    grep -oP '(?<=\(eV\): )[-.0-9]+' $file | tr -d '\n'
    printf "\n"
done
