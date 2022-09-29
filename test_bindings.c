
#include <stdio.h>
#include <stdlib.h>
#include "bgw_wfn_io.h"

int main(int argc, char *argv[]) {
	struct mf_header_t mf;
	f64_t *coeffs;
	i32_t *gvecs;

	printf("Reading input wavefunction WFN_in.h5\n");
	bgw_io_read_mf_header("WFN_in.h5", &mf);
	//bgw_io_print_mf_header(&mf);

	printf("Writing output wavefunction WFN_out.h5\n");
	bgw_io_setup_wfn("WFN_out.h5", &mf, 1);
	bgw_io_write_mf_header("WFN_out.h5", &mf);

	const int nb = mf.kpoints.mnband;
	const int nk = mf.kpoints.nrk;
	const int ns = mf.kpoints.nspin * mf.kpoints.nspinor;
	for (int ik=0; ik<nk; ++ik){
		const int ng_ik = mf.kpoints.ngk[ik];

		// Read G-vectors G for k-point ik
		gvecs = (i32_t*) malloc(ng_ik*3*sizeof(i32_t));
		bgw_io_read_gvecs_ik_serial("WFN_in.h5", &mf, ik, gvecs);
		bgw_io_write_gvecs_ik_serial("WFN_out.h5", &mf, ik, gvecs);
		free(gvecs);

		// Read wavefunciton coefficients c_{nk}(G) for k-point ik
		coeffs = (f64_t*) malloc(nb*ns*ng_ik*2*sizeof(f64_t));
		bgw_io_read_coeffs_ik_serial("WFN_in.h5", &mf, ik, coeffs);
		bgw_io_write_coeffs_ik_serial("WFN_out.h5", &mf, ik, coeffs);
		free(coeffs);
	}

	//bgw_io_print_mf_header(&mf);

	bgw_io_free_mf_header(&mf);
	printf("All done!\n");

	return 0;
}
