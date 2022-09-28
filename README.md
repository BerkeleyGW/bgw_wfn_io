# BGW WFN IO


## Instructions
Manually edit the `Makefile` and make the project. A library `bgw_wfn_io.so`
will be created.


## Testing
1. Run `./test_bindings.x` to test the library. It will read the input file
   `WFN_in.h5` and write the output file `WFN_out.h5`.
2. You can check the correctness of the program with
   `h5diff -v -c WFN_in.h5 WFN_out.h5 /`

## TODO
* Migrate to CMake.
* Add more docs.
* Implement parallel I/O for wavefunction coefficients (check distrib. w/ Alfredo).
* Add optional flag in the WFN file when the wavefunction is stored in the full
  FFT grid. Can optimize BGW for that case (e.g., Umklapp vectors -> cycle shift).
