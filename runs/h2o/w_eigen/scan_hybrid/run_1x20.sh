#!/bin/bash
#SBATCH -J libqqc_hybrid_1x20
#SBATCH -n1 --cpus-per-task=20 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_hybrid_1x20
#SBATCH --output=libqqc_hybrid_1x20_por-%j.out
#SBATCH --error=libqqc_hybrid_1x20_por-%j.err
pwd; hostname; date
cat run_1x20.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 1x20.out
OMP_NUM_THREADS=20 srun --cpus-per-task=$OMP_NUM_THREADS ./1x20.out
rm 1x20.out
