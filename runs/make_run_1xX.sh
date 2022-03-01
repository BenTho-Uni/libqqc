#!/bin/bash

if [ -z "$1" ]
then 
	echo 'Please specify ncores as $1' 
	exit 1
else
	NCORES=$1
fi

echo '#!/bin/bash'
echo '#SBATCH -J libqqc_mpionly_1x'$NCORES
echo '#SBATCH -N1 --ntasks=1 --cpus-per-task='$NCORES '-t12:00:00'
echo '#SBATCH --mem=100G'
echo '#SBATCH --job-name=libqqc_mpionly_1x'$NCORES
echo '#SBATCH --output=libqqc_mpionly_1x'$NCORES'_por-%j.out'
echo '#SBATCH --error=libqqc_mpionly_1x'$NCORES'_por-%j.err'
echo 'pwd; hostname; date'
echo 'cat run_1x'$NCORES'.sh'
echo 'module load compiler/gnu'
echo 'module load mpi/openmpi'
echo 'cd ../host_example/.'
echo 'mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o 1x'$NCORES'.out'
echo 'OMP_NUM_THREADS=1 srun --cpus-per-task=1 ./1x'$NCORES'.out'

echo 'rm 1x'$NCORES'.out'

