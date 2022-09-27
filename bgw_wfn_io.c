/* -*- indent-tabs-mode: t -*- */


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

void write_mf_header_kpoints(hid_t file, struct kpoints_t kp){
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nspin", 0, (hsize_t[]){0}, &kp.nspin);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nspinor", 0, (hsize_t[]){0}, &kp.nspinor);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nrk", 0, (hsize_t[]){0}, &kp.nrk);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/mnband", 0, (hsize_t[]){0}, &kp.mnband);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ngkmax", 0, (hsize_t[]){0}, &kp.ngkmax);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/ecutwfc", 0, (hsize_t[]){0}, &kp.ecutwfc);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/kgrid", 1, (hsize_t[]){3}, &kp.kgrid[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/shift", 1, (hsize_t[]){3}, &kp.shift[0]);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ngk", 1, (hsize_t[]){kp.nrk}, kp.ngk);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ifmin", 2, (hsize_t[]){kp.nspin,kp.nrk}, kp.ifmin);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ifmax", 2, (hsize_t[]){kp.nspin,kp.nrk}, kp.ifmax);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/w", 1, (hsize_t[]){kp.nrk}, kp.w);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/rk", 2, (hsize_t[]){kp.nrk,3}, kp.rk);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/el", 3, (hsize_t[]){kp.nspin,kp.nrk,kp.mnband}, kp.el);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/occ", 3, (hsize_t[]){kp.nspin,kp.nrk,kp.mnband}, kp.occ);
}

void write_mf_header_gspace(hid_t file, struct gspace_t gvec){
	H5LTmake_dataset_int(file, "/mf_header/gspace/ng", 0, (hsize_t[]){0}, &gvec.ng);
	H5LTmake_dataset_double(file, "/mf_header/gspace/ecutrho", 0, (hsize_t[]){0}, &gvec.ecutrho);
	H5LTmake_dataset_int(file, "/mf_header/gspace/FFTgrid", 1, (hsize_t[]){3}, &gvec.FFTgrid[0]);
	H5LTmake_dataset_int(file, "/mf_header/gspace/components", 2, (hsize_t[]){gvec.ng,3}, gvec.components);
}

void write_mf_header_symmetry(hid_t file, struct symmetry_t syms){
	H5LTmake_dataset_int(file, "/mf_header/symmetry/ntran", 0, (hsize_t[]){0}, &syms.ntran);
	H5LTmake_dataset_int(file, "/mf_header/symmetry/cell_symmetry", 0, (hsize_t[]){0}, &syms.cell_symmetry);
	H5LTmake_dataset_int(file, "/mf_header/symmetry/mtrx", 3, (hsize_t[]){48,3,3}, &syms.mtrx[0][0][0]);
	H5LTmake_dataset_double(file, "/mf_header/symmetry/tnp", 2, (hsize_t[]){48,3}, &syms.tnp[0][0]);
}

void write_mf_header_crystal(hid_t file, struct crystal_t crys){
	H5LTmake_dataset_double(file, "/mf_header/crystal/celvol", 0, (hsize_t[]){0}, &crys.celvol);
	H5LTmake_dataset_double(file, "/mf_header/crystal/recvol", 0, (hsize_t[]){0}, &crys.recvol);
	H5LTmake_dataset_double(file, "/mf_header/crystal/alat", 0, (hsize_t[]){0}, &crys.alat);
	H5LTmake_dataset_double(file, "/mf_header/crystal/blat", 0, (hsize_t[]){0}, &crys.blat);
	H5LTmake_dataset_int(file, "/mf_header/crystal/nat", 0, (hsize_t[]){0}, &crys.nat);
	H5LTmake_dataset_double(file, "/mf_header/crystal/avec", 2, (hsize_t[]){3,3}, &crys.avec[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/bvec", 2, (hsize_t[]){3,3}, &crys.bvec[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/adot", 2, (hsize_t[]){3,3}, &crys.adot[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/bdot", 2, (hsize_t[]){3,3}, &crys.bdot[0][0]);
	H5LTmake_dataset_int(file, "/mf_header/crystal/atyp", 1, (hsize_t[]){crys.nat}, crys.atyp);
	H5LTmake_dataset_double(file, "/mf_header/crystal/apos", 2, (hsize_t[]){crys.nat,3}, crys.apos);
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
