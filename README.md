# libqqc
*Contributing Authors:* Benjamin Thomitzni

| **Code** | [![][github-img]][github-url] [![][license-img]][license-url]|
| :----- | :----- |
| **Meta** | [![][ci-img]][ci-url] |
| **Documentation** | [![][docs-img]][docs-url] |

[docs-img]: https://img.shields.io/badge/docs-latest-blue
[docs-url]: https://bentho-uni.github.io/libqqc/
[github-img]: https://img.shields.io/badge/GitHub-source-brightgreen
[github-url]: https://github.com/BenTho-Uni/libqqc
[license-img]: https://img.shields.io/github/license/BenTho-Uni/libqqc
[license-url]: https://github.com/BenTho-Uni/libqqc/blob/main/LICENSE
[ci-img]: https://github.com/BenTho-Uni/libqqc/workflows/CI/badge.svg?branch=main&event=push
[ci-url]: https://github.com/BenTho-Uni/libqqc/actions

libqqc is a library for high performance evaluation of quantum chemistry methods through quadrature schemes.

## Table of Contents

- [libqqc](#libqqc)
    - [Overview](#overview)
        - [Performance](#performance)
        - [Contents](#contents)
    - [Installation](#installation)
    - [Usage](#usage)
    - [FAQ](#faq)
    - [ToDo](#todo)
- [Credits](#credits)

## Overview

### Performance

### Contents

#### ./github/workflows
Continious Integration and documentation deployment action workflows for github.

#### /data: example data
We provide H2O human-readable data which can be used for testing and a script 
to download larger test systems (H3COH, Anthracene, Porphyrin).

#### /docs: Doxygen documentation
Provides the doxygen documentation which is automatically build and deploys
through GitHub pages to [!](https://bentho-uni.github.io/libqqc/).

#### /host_example: example host setup
MPI and no-MPI example on how to setup and run calculation. 

Please be aware: For Hybrid (MPI+OpenMP) usage follow the example given and 
setup the correct `MPI_Init_thread` call

#### /libqqcs: library files
Location of library header and body files. 

#### /runs: run and data process scripts
Scripts used to run the library and process the data. Runs were done on 
[BwForCluster JUSTUS 2](https://wiki.bwhpc.de/e/Category:BwForCluster_JUSTUS_2).

#### /templates: template header and body files
Template files to speed up development.

#### /tests: testing suit
Testing suit set-up per module for both OpenMP, MPI and MPI+OpenMP versions.

## Installation

Packages needed
- for building `cmake, gcc`
- for documentation `doxygen`
- for MPI `mpich`
- for Eigen `Eigen`

Setup is handled by `configure` script, see

```
./configure -h
```

for help. Current options:
- `gcc` : Default, Build with the GNU compiler
- `noOpenMP` : Deactivate OpenMP
- `wMPI` : compile with MPI wrappers to enable MPI
- `wEigen` : compile `Eigen` library versions of calculations, needs 
`EIGEN3_INCLUDE_DIR` set, e.g.
```
EIGEN3_INCLUDE_DIR=~/bin/eigen-3.4.0/Eigen/
```
- `wGPU` : enables GPU versions of calculations, NOT SUPPORTED YET

after `configure`, change to build directory, and compile with `make`
```
cd build
make
make install
```

`make install` installs library and exe into upper `/lib` and `/exe` folder.

You then can compile the host program, for example with `/host_example/`
```
gcc -fopenmp main.cpp ../lib/libqqc.a
```
or 
```
mpic++ -fopenmp main_mpi.cpp ../lib/libqqc.a
```

The library has been tested with GNU gcc/g++ 10.1.0

## Usage
To run a calculation you have to set up three distinct parts. 

1) `Loader_*`
Loaders are method and host program specific and are used to setup the routins
for the loading of the necessary data. MPI and no-MPI versions differ in these 
loading methods.

2) `Vault_*`
Vaults are method specific and hold only the necessary data for the calculation.
They are given the `Loader_*` object through their constructor, from which they
call the loading methods. Therefore the constructor is specific to the `Loader_*`
object.

3) `Do_*`
These objects do the set-up for the calculation and then execute it. Results are 
printed into a output stream. They are given the `Vault_*` data holder object
and acces the data through them.

See `/host_example/main.cpp` for an example on how to call the library. 

## FAQ
tba

## ToDo

<details>
    <summary> finished ToDos</summary>

- [x] add primitive libr. structure
- [x] add doxygen
- [x] add workflow to build and deploy doxygen doc to github pages
- [x] add files to test cmake setup
- [x] add cmake setup for library
- [x] add cmake setup for test
- [x] add templates for libqqc/ and tests/ files for correct structure/documentation
- [x] add cmake structure for w/o Eigen, w/o OpenMP GPU offloading and  w/o MPI variant compiling (g++/mpic++)
- [x] add workflow for testing after commit
- [x] add testing workflow for MPI, Eigen etc. variants
- [x] add prelim. loader objects
- [x] add timing object
- [x] add grid object
- [x] add vault object
- [x] add real loader
- [x] add data/h2o with bashscripts to download h3coh/, anthracene/, porphyrine/
- [x] transfer MP2 variants to libqqc
- [x] partitioning scheme for remaining elements in MPI
- [x] remove unneded balast from qmp2 calc, loader, vault
- [x] write printer
- [x] write/rewrite test for printer, vault, loader, do_qmp2
- [x] fit max item sharing (bytes?), _on_i ?
- [x] fix max item in loader
- [x] fix false sharing
- [x] recheck performance on Justus 2.0
    - [x] time resolved average cpu utilization on 4 nodes
    - [x] core scaling 1-960 for Hybrid for do_mp2 and coulomb trafo
    - [x] core scaling 1-960 for MPI only for do_mp2 and coulomb trafo

### Notes from MPI+Hybrid course
- [ ] set MPI_init to MPI_init_thread with MPI_THREAD_FUNNELED and argcs
</details>

- [ ] add missing badges
- [ ] add input/output of host programm which was used to export to testdata
- [ ] update Readme with scalings and results from Justus 2.0
- [ ] write better maintainable variant code
- [ ] OpenMP GPU variant implementation
- [ ] rewrite send/recv into bcast
- [ ] MPI distri for qmp2 constructor scaling loops
- [ ] move to hdf5?
- [ ] redo interface Q-Chem
- [ ] interface python through pybind11
- [ ] decide how digesters and integrals license

# Credits

The [Doxygen](https://www.doxygen.nl/index.html) documentation uses the 
[doxygen-awesome](https://jothepro.github.io/doxygen-awesome-css/index.html) theme.

Badges by [Shields.io](https://shields.io).
