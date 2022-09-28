/* -*- indent-tabs-mode: t -*- */
<%from bgw_h5_spec_to_c import create_write_cmd, create_read_cmd, create_print_cmd%>

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "bgw_wfn_io.h"
#include "hdf5.h"
#include "hdf5_hl.h"


/*
 * Internal high-level routines
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
		create_group(file, "/mf_header/crystal");
	}
}


void setup_wfns(hid_t file){
	if (!H5Lexists(file, "/wfns", H5P_DEFAULT)) {
		create_group(file, "/wfns");
	}
}


/*
 * Routines to write mf_header
 */


void write_mf_header_kpoints(hid_t file, struct kpoints_t *kp){
${create_write_cmd('wfn.h5.spec', 'kpoints')}
}

void write_mf_header_gspace(hid_t file, struct gspace_t *gvec){
${create_write_cmd('wfn.h5.spec', 'gspace')}
}

void write_mf_header_symmetry(hid_t file, struct symmetry_t *syms){
${create_write_cmd('wfn.h5.spec', 'symmetry')}
}

void write_mf_header_crystal(hid_t file, struct crystal_t *crys){
${create_write_cmd('wfn.h5.spec', 'crystal')}
}


/*
 * Routines to read mf_header
 */


void read_mf_header_kpoints(hid_t file, struct kpoints_t *kp){
${create_read_cmd('wfn.h5.spec', 'kpoints')}
}

void read_mf_header_gspace(hid_t file, struct gspace_t *gvec){
${create_read_cmd('wfn.h5.spec', 'gspace')}
}

void read_mf_header_symmetry(hid_t file, struct symmetry_t *syms){
${create_read_cmd('wfn.h5.spec', 'symmetry')}
}

void read_mf_header_crystal(hid_t file, struct crystal_t *crys){
${create_read_cmd('wfn.h5.spec', 'crystal')}
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

void bgw_io_write_mf_header(const char *fname, struct mf_header_t *mf){
	hid_t file;

	file = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	setup_mf_header(file);
	H5LTmake_dataset_int(file, "/mf_header/versionnumber", 0, (hsize_t[]){0}, &mf->versionnumber);
	H5LTmake_dataset_int(file, "/mf_header/flavor", 0, (hsize_t[]){0}, &mf->flavor);
	write_mf_header_kpoints(file, &mf->kpoints);
	write_mf_header_gspace(file, &mf->gspace);
	write_mf_header_symmetry(file, &mf->symmetry);
	write_mf_header_crystal(file, &mf->crystal);
	H5Fclose(file);
}

void bgw_io_read_mf_header(const char *fname, struct mf_header_t *mf){
	hid_t file;

	file = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
	H5LTread_dataset_int(file, "/mf_header/versionnumber", &mf->versionnumber);
	H5LTread_dataset_int(file, "/mf_header/flavor", &mf->flavor);
	read_mf_header_kpoints(file, &mf->kpoints);
	read_mf_header_gspace(file, &mf->gspace);
	read_mf_header_symmetry(file, &mf->symmetry);
	read_mf_header_crystal(file, &mf->crystal);
	H5Fclose(file);
}
