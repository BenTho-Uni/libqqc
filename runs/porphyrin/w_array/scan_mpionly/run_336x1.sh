#!/bin/bash
#SBATCH -J libqqc_mpionly_336x1
#SBATCH -n336 --cpus-per-task=1 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_mpionly_336x1
#SBATCH --output=libqqc_mpionly_336x1_por-%j.out
#SBATCH --error=libqqc_mpionly_336x1_por-%j.err
pwd; hostname; date
cat run_336x1.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 336x1.out
OMP_NUM_THREADS=1 srun --cpus-per-task=$OMP_NUM_THREADS ./336x1.out
rm 336x1.out
