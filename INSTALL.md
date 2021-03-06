This project uses [`cmake`](https://cmake.org/) as build system.

# Dependencies

## Required dependencies

- [`MFEM`](https://mfem.org/)
- [`MGIS`](https://github.com/thelfer/MFrontGenericInterfaceSupport)

A simple way to install dependencies is to rely on `Spack` system. 
Spack is an open source package manager that simplifies building, installing, customizing, and sharing HPC software.
It will allow you to install recent versions of compilers (that handle c++17, for example gnu compiler suite version 8),
and to get python, cmake and other tools that are required for this project to be installed.

~~~~{.bash}
    git clone https://github.com/spack/spack.git
    export SPACK_ROOT=$PWD/spack
    source ${SPACK_ROOT}/share/spack/setup-env.sh
    spack install hypre metis mgis@master cmake
    spack load hypre metis mgis@master cmake

    git clone https://github.com/mfem/mfem.git
    # or download a tarball here : https://mfem.org/download/
    cd mfem
    mkdir build; cd build
    cmake ../ -DCMAKE_INSTALL_PREFIX=$PWD/mfem -DCMAKE_CXX_COMPILER=g++ 
    make -j 4
    make install
    export MFEM_DIR=$PWD/mfem
~~~~

## Optional dependencies

The `TFEL` project can be used for testing purposes.

# Relevant variables

- `CMAKE_BUILD_TYPE`: type of build
- `CMAKE_INSTALL_PREFIX`: installation prefix
- `MFEM_DIR` must be set to the location where `MFEMConfig.cmake` has
  been installed. To get this file, one needs to compile MFEM with CMake
  compilation process and launch a `make install` finally.
- `MFrontGenericInterface_DIR` must be set to the location where
  `MFrontGenericInterfaceConfig.cmake` has been installed.

# Example of usage

- Suppose that you install `mgis` using spack. For example with the command `spack install mgis@master`.
~~~~{.bash}
cmake .. -DCMAKE_BUILD_TYPE=Release  -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
   -DCMAKE_INSTALL_PREFIX=$PWD/../install -DMFEM_DIR=<MFEM_DIR> \
   -DMFrontGenericInterface_DIR=$(spack location -i mgis@master)/share/mgis/cmake

make -j 4
make check
~~~~