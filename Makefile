
# module load cray-hdf5-parallel
HDF5DIR = $(HDF5_DIR)

LINK = mpicc
#LDFLAGS = -L$(HDF5DIR)/lib -lhdf5
CC = mpicc
LDFLAGS = -shared -L$(TACC_HDF5_LIB) -lhdf5 -lhdf5_hl
CFLAGS = --warn-all -fPIC -I$(TACC_HDF5_INC)

all: lib bgw_wfn_io.o

lib: bgw_wfn_io.so

bgw_wfn_io.so: bgw_wfn_io.o
	$(CC) -o $@ $< $(LDFLAGS)

bgw_wfn_io.o: bgw_wfn_io.c bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.x *.o

.PHONY: clean lib all
