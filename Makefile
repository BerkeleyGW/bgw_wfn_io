
# module load cray-hdf5-parallel
HDF5DIR = $(HDF5_DIR)

LINK = mpicc
CC = mpicc

LDFLAGS = -L$(TACC_HDF5_LIB) -lhdf5 -lhdf5_hl -ggdb3 -Wall -fvisibility=default -fstack-protector-all -fstack-check
CFLAGS = -fPIC -I$(TACC_HDF5_INC) -ggdb3 -Wall -fvisibility=default -fstack-protector-all -fstack-check -Iinclude/

all: lib test
lib: src/libbgw_wfn_io.so
test: tests/test_bindings.x

src/libbgw_wfn_io.so: src/bgw_wfn_io.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

tests/test_bindings.x: tests/test_bindings.o src/bgw_wfn_io.o
	$(CC) -o $@ $^ $(LDFLAGS)

tests/test_bindings.o: tests/test_bindings.c include/bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/bgw_wfn_io.o: src/bgw_wfn_io.c include/bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f */*.x */*.o

.PHONY: clean all lib test
