
#include <stdio.h>
#include "bgw_wfn_io.h"

int main(int argc, char *argv[]) {
	struct mf_header_t mf;

	bgw_io_read_mf_header("WFN_in.h5", &mf);
	printf("nrk: %d\n", mf.kpoints.nrk);
	bgw_io_setup_wfn("WFN_out.h5");
	bgw_io_write_mf_header("WFN_out.h5", &mf);

	return 0;
}
