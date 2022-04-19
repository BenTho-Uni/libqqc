#!/bin/bash
#SBATCH -J libqqc_hybrid_17x48
#SBATCH -n17 --cpus-per-task=48 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_hybrid_17x48
#SBATCH --output=libqqc_hybrid_17x48_por-%j.out
#SBATCH --error=libqqc_hybrid_17x48_por-%j.err
pwd; hostname; date
cat run_17x48.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 17x48.out
OMP_NUM_THREADS=48 srun --cpus-per-task=$OMP_NUM_THREADS ./17x48.out
rm 17x48.out
