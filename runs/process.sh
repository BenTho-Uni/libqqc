#!/bin/bash
# This script takes the output of libqqc runs and prints 
# a csv of the timings and some other infos

# we loop over all files given as arguments
for file in "$@"; do
    printf $file # filename
    printf "," 
    # the following prints the output of pdf;hostname;date in the output
    awk '{if(NR==3) print $0}' $file | sed 's/ //g' | tr -d '\n'
    printf ","
    # grep the number after the search term
    grep -oP '(?<=threads:   )[0-9,]+' $file | tr -d '\n'
    printf ","
    grep -oP '(?<=processes: )[0-9,]+' $file | tr -d '\n'
    printf ","
    # Grep all timings, put them in csv format
    while read -r line; do
        printf $line
        printf ","
    done < <(grep -oP '(?<=Wall: )[0-9]+' $file)
    # Finally, grep the energy just to be sure that the calculations
    # worked
    grep -oP '(?<=\(eV\): )[-.0-9]+' $file | tr -d '\n'
    printf "\n"
done
