#!/bin/bash
#SBATCH -J libqqc_mpionly_480x1
#SBATCH -n480 --cpus-per-task=1 -t12:00:00
#SBATCH --mem=200G
#SBATCH --job-name=libqqc_mpionly_480x1
#SBATCH --output=libqqc_mpionly_480x1_por-%j.out
#SBATCH --error=libqqc_mpionly_480x1_por-%j.err
pwd; hostname; date
cat run_480x1.sh
module load compiler/gnu
module load mpi/openmpi
cd ../../host_example/.
mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 480x1.out
OMP_NUM_THREADS=1 srun --cpus-per-task=$OMP_NUM_THREADS ./480x1.out
rm 480x1.out
