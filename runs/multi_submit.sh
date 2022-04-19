#!/bin/bash
for file in "$@"; do
    sbatch $file
done
