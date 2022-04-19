#!/bin/bash
#SBATCH -J libqqc_hybrid_1x40
#SBATCH -n1 --cpus-per-task=40 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_hybrid_1x40
#SBATCH --output=libqqc_hybrid_1x40_por-%j.out
#SBATCH --error=libqqc_hybrid_1x40_por-%j.err
pwd; hostname; date
cat run_1x40.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 1x40.out
OMP_NUM_THREADS=40 srun --cpus-per-task=$OMP_NUM_THREADS ./1x40.out
rm 1x40.out
