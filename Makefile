
# module load cray-hdf5-parallel
HDF5DIR = $(HDF5_DIR)

LINK = mpicc
CC = mpicc

LDFLAGS = -L$(TACC_HDF5_LIB) -lhdf5 -lhdf5_hl -ggdb3 -Wall -fvisibility=default -fstack-protector-all -fstack-check
CFLAGS = -fPIC -I$(TACC_HDF5_INC) -ggdb3 -Wall -fvisibility=default -fstack-protector-all -fstack-check

all: lib test
lib: libbgw_wfn_io.so
test: test_bindings.x

libbgw_wfn_io.so: bgw_wfn_io.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

test_bindings.x: test_bindings.o bgw_wfn_io.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_bindings.o: test_bindings.c bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

bgw_wfn_io.o: bgw_wfn_io.c bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.x *.o

.PHONY: clean all lib test
