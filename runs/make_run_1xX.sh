#!/bin/bash

if [ -z "$1" ]
then 
	echo 'Please specify ncores as $1' 
	exit 1
else
	NCORES=$1
fi

echo '#!/bin/bash'
echo '#SBATCH -J jove_mpi_test_1x'$NCORES
echo '#SBATCH -N1 --ntasks=1 --cpus-per-task='$NCORES '--mem-per-cpu=10G -t12:00:00'
echo '#SBATCH --job-name=jove_mpi_test_1x'$NCORES
echo '#SBATCH --output=jove_mpi_test_1x'$NCORES'_por-%j.out'
echo '#SBATCH --error=jove_mpi_test_1x'$NCORES'_por-%j.err'
echo 'cat run_1x'$NCORES'.sh'
echo 'module load compiler/gnu'
echo 'module load mpi/openmpi'
echo 'mpic++ -fopenmp -O3 -ffast-math -march=native test_mpi.C -o 1x'$NCORES'.out'
echo 'OMP_PLACES=cores OMP_PROC_BIND=spread OMP_NUM_THREADS='$NCORES' mpirun -np 1 --bind-to core --map-by ppr:1:node:pe='$NCORES' -x OMP_NUM_THREADS -x OMP_PROC_BIND -x OMP_PLACES ./1x'$NCORES'.out'

echo 'rm 1x'$NCORES'.out'

