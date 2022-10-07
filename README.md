# BGW WFN IO

A pure C library to read and write wavefunction files in HDF5 format according
to the BerkeleyGW format [specification](http://manual.berkeleygw.org/3.0/wfn_h5_spec/).


## Instructions

You should build the code using either CMake or by adjusting the provided
Makefile template.

### Using CMake
Follow these steps:
```
mkdir build && cd build
cmake ../
make
make test
```
You may need to manually specify the compiler with the `CC` flag, e.g.,
`CC=mpicc cmake ../`.

### Manually editting the sample `Makefile`
Manually edit the `Makefile` and make the project. A library `libbgw_wfn_io.so`
will be created.


## Testing
1. Run `./test_bindings.x` to test the library. It will read the input file
   `WFN_in.h5` and write the output file `WFN_out.h5`.
2. You can check the correctness of the program with
   `h5diff -v -c WFN_in.h5 WFN_out.h5 /`


## TODO
* Improve CMake support.
* Improve documentation.
* Implement parallel I/O for wavefunction coefficients (check distrib. w/ Alfredo).
* Add optional flag in the WFN file when the wavefunction is stored in the full
  FFT grid. Can optimize BGW for that case (e.g., Umklapp vectors -> cycle shift).


## Developers
* Used CMake structure from: https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project.
