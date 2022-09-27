#!/usr/bin/env python3

from bgw_wfn import MF_HEADER, KPOINTS, GSPACE, SYMMETRY, CRYSTAL
from ctypes import cdll


def main(fname_in):
    lib = cdll.LoadLibrary('./bgw_wfn_io.so')
    lib.bgw_io_setup_wfn(b'WFN_out.h5')
    print(lib)

if __name__ == "__main__":
    main('WFN_in.h5')
