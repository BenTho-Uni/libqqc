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

#### /data: Example Data and Runs (Justus 2.0)

#### /docs: Doxygen Documentation

#### /libqqcs: Library Location

#### /tests: Testing Suit 

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

after `cofigure`, change to build directory, and compile with `make`
```
cd build
make
make install
```

`make install` installs library and exe into upper `/lib` and `/exe` folder.

## Usage

## FAQ

## TODO

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
- [ ] add missing badges
- [ ] add data/h2o with bashscripts to download h3coh/, anthracene/, porphyrine/
- [x] transfer MP2 variants to libqqc
- [ ] recheck performance on Justus 2.0
- [ ] update Readme with scalings and results from Justus 2.0
- [ ] new partitioning scheme to minimize network IO by partitioning virtual space
- [ ] write better maintainable variant code
- [ ] remove unneded balast from qmp2 calc, loader, vault
- [ ] partitioning scheme for remaining elements in MPI
- [ ] OpenMP GPU variant implementation

# Credits

The [Doxygen](https://www.doxygen.nl/index.html) documentation uses the 
[doxygen-awesome](https://jothepro.github.io/doxygen-awesome-css/index.html) theme.

Badges by [Shields.io](https://shields.io).
