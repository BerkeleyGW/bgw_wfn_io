#!/usr/bin/env python3

from bgw_wfn import MF_HEADER, KPOINTS, GSPACE, SYMMETRY, CRYSTAL
from ctypes import cdll, pointer, POINTER


def main(fname_in):
    lib = cdll.LoadLibrary('./bgw_wfn_io.so')
    mf = MF_HEADER()
    lib.bgw_io_read_mf_header(b'WFN_in.h5', pointer(mf))
    #lib.bgw_io_setup_wfn(b'WFN_out.h5')
    #lib.bgw_io_write_mf_header(b'WFN_in.h5', mf)


if __name__ == "__main__":
    main('WFN_in.h5')
