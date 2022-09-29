#!/usr/bin/env python3

from bgw_wfn import MF_HEADER, i32_t, f64_t
from ctypes import cdll, byref


def main(fname_in):
    lib = cdll.LoadLibrary('./libbgw_wfn_io.so')
    mf = MF_HEADER()

    print('Reading input wavefunction WFN_in.h5')
    lib.bgw_io_read_mf_header(b'WFN_in.h5', byref(mf))

    print('Writing output wavefunction WFN_out.h5')
    lib.bgw_io_setup_wfn(b'WFN_out_py.h5', byref(mf), 1)
    lib.bgw_io_write_mf_header(b'WFN_out_py.h5', byref(mf))

    ns = mf.kpoints.nspinor * mf.kpoints.nspin
    nb = mf.kpoints.mnband
    for ik in range(mf.kpoints.nrk):
        ng_ik = mf.kpoints.ngk[ik]

        # Read G-vectors G for k-point ik
        gvecs = (ng_ik*3*i32_t)()
        lib.bgw_io_read_gvecs_ik_serial(b'WFN_in.h5', byref(mf), ik, gvecs)
        lib.bgw_io_write_gvecs_ik_serial(b'WFN_out_py.h5', byref(mf), ik, gvecs)
        del gvecs

        # Read wavefunciton coefficients c_{nk}(G) for k-point ik
        coeffs = (nb*ns*ng_ik*2*f64_t)()
        lib.bgw_io_read_coeffs_ik_serial(b'WFN_in.h5', byref(mf), ik, coeffs)
        lib.bgw_io_write_coeffs_ik_serial(b'WFN_out_py.h5', byref(mf), ik, coeffs)
        del coeffs

    lib.bgw_io_free_mf_header(byref(mf))
    print('All done!')


if __name__ == "__main__":
    main('WFN_in.h5')
