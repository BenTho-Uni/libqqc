# libqqc
*Contributing Authors:* Benjamin Thomitzni

| **Code** | [![][github-img]][github-url]  [![][license-img]][license-url]|
| :----- | :----- |
| **Documentation** | [![][docs-img]][docs-url] |

[docs-img]: https://img.shields.io/badge/docs-latest-blue?style=flat-square
[docs-url]: https://bentho-uni.github.io/libqqc/
[github-img]: https://img.shields.io/badge/GitHub-source-brightgreen?style=flat-square
[github-url]: https://github.com/BenTho-Uni/libqqc
[license-img]: https://img.shields.io/github/license/BenTho-Uni/libqqc?style=flat-square
[license-url]: https://github.com/BenTho-Uni/libqqc/blob/main/LICENSE

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

#### /test: Testing Suit 

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
- `gcc` : Build with the GNU compiler
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
- [ ] add templates for libqqc/ and test/ files for correct structure/documentation
- [ ] add cmake structure for w/o Eigen, w/o OpenMP GPU offloading and  w/o MPI variant compiling (g++/mpic++)
- [ ] add workflow for testing after commit
- [ ] add missing badges
- [ ] add data/h2o with bashscripts to download h3coh/, anthracene/, porphyrine/
- [ ] transfer MP2 variants to libqqc
- [ ] recheck performance on Justus 2.0
- [ ] update Readme with scalings and results from Justus 2.0

# Credits

The [Doxygen](https://www.doxygen.nl/index.html) documentation uses the 
[doxygen-awesome](https://jothepro.github.io/doxygen-awesome-css/index.html) theme.

Badges by [Shields.io](https://shields.io).
