/* -*- indent-tabs-mode: t -*- */

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "bgw_wfn_io.h"
#include "hdf5.h"
#include "hdf5_hl.h"


/*
 * High-level 
 */


void create_group(hid_t file, const char *group_name){
	hid_t group;
	group = H5Gcreate2(file, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Gclose(group);
}


void setup_mf_header(hid_t file){
	if (!H5Lexists(file, "/mf_header", H5P_DEFAULT)) {
		create_group(file, "/mf_header");
		create_group(file, "/mf_header/kpoints");
		create_group(file, "/mf_header/gspace");
		create_group(file, "/mf_header/symmetry");
	}
}


void setup_wfns(hid_t file){
	if (!H5Lexists(file, "/wfns", H5P_DEFAULT)) {
		create_group(file, "/wfns");
	}
}


void write_mf_header_gspace(hid_t file, struct gspace_t gspace){
	H5LTmake_dataset_int(file, "/mf_header/gspace/ng", 0, (hsize_t[]){0}, &gspace.ng);
	H5LTmake_dataset_double(file, "/mf_header/gspace/ecutrho", 0, (hsize_t[]){0}, &gspace.ecutrho);
	H5LTmake_dataset_int(file, "/mf_header/gspace/FFTgrid", 1, (hsize_t[]){3}, gspace.FFTgrid);
	H5LTmake_dataset_int(file, "/mf_header/gspace/components", 2, (hsize_t[]){gspace.ng,3}, gspace.components);
}


/*
 * Functions called by user
 */


void bgw_io_setup_wfn(const char *fname){
	hid_t file;
    	file = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	setup_mf_header(file);
	H5Fclose(file);
}

void bgw_io_write_mf_header(const char *fname, struct mf_header_t mf_header){
	hid_t file;

    	file = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	setup_mf_header(file);
	write_mf_header_gspace(file, mf_header.gspace);
	H5Fclose(file);
}
