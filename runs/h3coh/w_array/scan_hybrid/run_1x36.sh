#!/bin/bash
#SBATCH -J libqqc_hybrid_1x36
#SBATCH -n1 --cpus-per-task=36 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_hybrid_1x36
#SBATCH --output=libqqc_hybrid_1x36_por-%j.out
#SBATCH --error=libqqc_hybrid_1x36_por-%j.err
pwd; hostname; date
cat run_1x36.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 1x36.out
OMP_NUM_THREADS=36 srun --cpus-per-task=$OMP_NUM_THREADS ./1x36.out
rm 1x36.out
