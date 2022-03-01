#!/bin/bash

if [ -z "$1" ]
then 
	echo 'Please specify nnodes as $1' 
	exit 1
else
	export NNODES=$1
fi

if [ -z "$2" ]
then
	echo 'Please specify ncores as $2'
	exit 1
else
	export NCORES=$2
fi


echo '#!/bin/bash'
echo '#SBATCH -J libqqc_hybrid_'$NNODES'x'$NCORES
echo '#SBATCH -n'$NNODES' --cpus-per-task='$NCORES '-t12:00:00'
echo '#SBATCH --mem=100G'
echo '#SBATCH --job-name=libqqc_hybrid_'$NNODES'x'$NCORES
echo '#SBATCH --output=libqqc_hybrid_'$NNODES'x'$NCORES'_por-%j.out'
echo '#SBATCH --error=libqqc_hybrid_'$NNODES'x'$NCORES'_por-%j.err'
echo 'pwd; hostname; date'
echo 'cat run_'$NNODES'x'$NCORES'.sh'
echo 'module load compiler/gnu'
echo 'module load mpi/openmpi'
echo 'cd ../host_example/.'
echo 'mpic++ -fopenmp -O3 -ffast-math -march=native main_mpi.cpp ../lib/libqqc.a -o '$NNODES'x'$NCORES'.out'
echo 'OMP_NUM_THREADS='$NCORES' srun --cpus-per-task=$OMP_NUM_THREADS ./'$NNODES'x'$NCORES'.out'

echo 'rm '$NNODES'x'$NCORES'.out'

