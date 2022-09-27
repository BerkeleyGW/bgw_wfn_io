
# module load cray-hdf5-parallel
HDF5DIR = $(HDF5_DIR)

LINK = mpicc
#LDFLAGS = -L$(HDF5DIR)/lib -lhdf5
CC = mpicc
CFLAGS = --warn-all

all: bgw_wfn_io.o

bgw_wfn_io.o: bgw_wfn_io.c bgw_wfn_io.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.x *.o

.PHONY: clean
