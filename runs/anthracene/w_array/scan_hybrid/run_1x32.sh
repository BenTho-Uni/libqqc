#!/bin/bash
#SBATCH -J libqqc_hybrid_1x32
#SBATCH -n1 --cpus-per-task=32 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_hybrid_1x32
#SBATCH --output=libqqc_hybrid_1x32_por-%j.out
#SBATCH --error=libqqc_hybrid_1x32_por-%j.err
pwd; hostname; date
cat run_1x32.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 1x32.out
OMP_NUM_THREADS=32 srun --cpus-per-task=$OMP_NUM_THREADS ./1x32.out
rm 1x32.out
