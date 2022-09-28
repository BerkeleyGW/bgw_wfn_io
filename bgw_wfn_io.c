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


void write_mf_header_kpoints(hid_t file, struct kpoints_t *kp){
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nspin", 0, (hsize_t[]){0}, &kp->nspin);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nspinor", 0, (hsize_t[]){0}, &kp->nspinor);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/nrk", 0, (hsize_t[]){0}, &kp->nrk);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/mnband", 0, (hsize_t[]){0}, &kp->mnband);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ngkmax", 0, (hsize_t[]){0}, &kp->ngkmax);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/ecutwfc", 0, (hsize_t[]){0}, &kp->ecutwfc);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/kgrid", 1, (hsize_t[]){3}, &kp->kgrid[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/shift", 1, (hsize_t[]){3}, &kp->shift[0]);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ngk", 1, (hsize_t[]){kp->nrk}, &kp->ngk[0]);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ifmin", 2, (hsize_t[]){kp->nspin,kp->nrk}, &kp->ifmin[0]);
	H5LTmake_dataset_int(file, "/mf_header/kpoints/ifmax", 2, (hsize_t[]){kp->nspin,kp->nrk}, &kp->ifmax[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/w", 1, (hsize_t[]){kp->nrk}, &kp->w[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/rk", 2, (hsize_t[]){kp->nrk,3}, &kp->rk[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/el", 3, (hsize_t[]){kp->nspin,kp->nrk,kp->mnband}, &kp->el[0]);
	H5LTmake_dataset_double(file, "/mf_header/kpoints/occ", 3, (hsize_t[]){kp->nspin,kp->nrk,kp->mnband}, &kp->occ[0]);
}

void write_mf_header_gspace(hid_t file, struct gspace_t *gvec){
	H5LTmake_dataset_int(file, "/mf_header/gspace/ng", 0, (hsize_t[]){0}, &gvec->ng);
	H5LTmake_dataset_double(file, "/mf_header/gspace/ecutrho", 0, (hsize_t[]){0}, &gvec->ecutrho);
	H5LTmake_dataset_int(file, "/mf_header/gspace/FFTgrid", 1, (hsize_t[]){3}, &gvec->FFTgrid[0]);
	H5LTmake_dataset_int(file, "/mf_header/gspace/components", 2, (hsize_t[]){gvec->ng,3}, &gvec->components[0]);
}

void write_mf_header_symmetry(hid_t file, struct symmetry_t *syms){
	H5LTmake_dataset_int(file, "/mf_header/symmetry/ntran", 0, (hsize_t[]){0}, &syms->ntran);
	H5LTmake_dataset_int(file, "/mf_header/symmetry/cell_symmetry", 0, (hsize_t[]){0}, &syms->cell_symmetry);
	H5LTmake_dataset_int(file, "/mf_header/symmetry/mtrx", 3, (hsize_t[]){48,3,3}, &syms->mtrx[0][0][0]);
	H5LTmake_dataset_double(file, "/mf_header/symmetry/tnp", 2, (hsize_t[]){48,3}, &syms->tnp[0][0]);
}

void write_mf_header_crystal(hid_t file, struct crystal_t *crys){
	H5LTmake_dataset_double(file, "/mf_header/crystal/celvol", 0, (hsize_t[]){0}, &crys->celvol);
	H5LTmake_dataset_double(file, "/mf_header/crystal/recvol", 0, (hsize_t[]){0}, &crys->recvol);
	H5LTmake_dataset_double(file, "/mf_header/crystal/alat", 0, (hsize_t[]){0}, &crys->alat);
	H5LTmake_dataset_double(file, "/mf_header/crystal/blat", 0, (hsize_t[]){0}, &crys->blat);
	H5LTmake_dataset_int(file, "/mf_header/crystal/nat", 0, (hsize_t[]){0}, &crys->nat);
	H5LTmake_dataset_double(file, "/mf_header/crystal/avec", 2, (hsize_t[]){3,3}, &crys->avec[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/bvec", 2, (hsize_t[]){3,3}, &crys->bvec[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/adot", 2, (hsize_t[]){3,3}, &crys->adot[0][0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/bdot", 2, (hsize_t[]){3,3}, &crys->bdot[0][0]);
	H5LTmake_dataset_int(file, "/mf_header/crystal/atyp", 1, (hsize_t[]){crys->nat}, &crys->atyp[0]);
	H5LTmake_dataset_double(file, "/mf_header/crystal/apos", 2, (hsize_t[]){crys->nat,3}, &crys->apos[0]);
}


/*
 * Routines to read mf_header
 */


void read_mf_header_kpoints(hid_t file, struct kpoints_t *kp){
	H5LTread_dataset_int(file, "/mf_header/kpoints/nspin", &kp->nspin);
	H5LTread_dataset_int(file, "/mf_header/kpoints/nspinor", &kp->nspinor);
	H5LTread_dataset_int(file, "/mf_header/kpoints/nrk", &kp->nrk);
	H5LTread_dataset_int(file, "/mf_header/kpoints/mnband", &kp->mnband);
	H5LTread_dataset_int(file, "/mf_header/kpoints/ngkmax", &kp->ngkmax);
	H5LTread_dataset_double(file, "/mf_header/kpoints/ecutwfc", &kp->ecutwfc);
	H5LTread_dataset_int(file, "/mf_header/kpoints/kgrid", &kp->kgrid[0]);
	H5LTread_dataset_double(file, "/mf_header/kpoints/shift", &kp->shift[0]);
	kp->ngk = (i32_t*) malloc(kp->nrk*sizeof(i32_t));
	H5LTread_dataset_int(file, "/mf_header/kpoints/ngk", &kp->ngk[0]);
	kp->ifmin = (i32_t*) malloc(kp->nspin*kp->nrk*sizeof(i32_t));
	H5LTread_dataset_int(file, "/mf_header/kpoints/ifmin", &kp->ifmin[0]);
	kp->ifmax = (i32_t*) malloc(kp->nspin*kp->nrk*sizeof(i32_t));
	H5LTread_dataset_int(file, "/mf_header/kpoints/ifmax", &kp->ifmax[0]);
	kp->w = (f64_t*) malloc(kp->nrk*sizeof(f64_t));
	H5LTread_dataset_double(file, "/mf_header/kpoints/w", &kp->w[0]);
	kp->rk = (f64_t*) malloc(kp->nrk*3*sizeof(f64_t));
	H5LTread_dataset_double(file, "/mf_header/kpoints/rk", &kp->rk[0]);
	kp->el = (f64_t*) malloc(kp->nspin*kp->nrk*kp->mnband*sizeof(f64_t));
	H5LTread_dataset_double(file, "/mf_header/kpoints/el", &kp->el[0]);
	kp->occ = (f64_t*) malloc(kp->nspin*kp->nrk*kp->mnband*sizeof(f64_t));
	H5LTread_dataset_double(file, "/mf_header/kpoints/occ", &kp->occ[0]);
}

void read_mf_header_gspace(hid_t file, struct gspace_t *gvec){
	H5LTread_dataset_int(file, "/mf_header/gspace/ng", &gvec->ng);
	H5LTread_dataset_double(file, "/mf_header/gspace/ecutrho", &gvec->ecutrho);
	H5LTread_dataset_int(file, "/mf_header/gspace/FFTgrid", &gvec->FFTgrid[0]);
	gvec->components = (i32_t*) malloc(gvec->ng*3*sizeof(i32_t));
	H5LTread_dataset_int(file, "/mf_header/gspace/components", &gvec->components[0]);
}

void read_mf_header_symmetry(hid_t file, struct symmetry_t *syms){
	H5LTread_dataset_int(file, "/mf_header/symmetry/ntran", &syms->ntran);
	H5LTread_dataset_int(file, "/mf_header/symmetry/cell_symmetry", &syms->cell_symmetry);
	H5LTread_dataset_int(file, "/mf_header/symmetry/mtrx", &syms->mtrx[0][0][0]);
	H5LTread_dataset_double(file, "/mf_header/symmetry/tnp", &syms->tnp[0][0]);
}

void read_mf_header_crystal(hid_t file, struct crystal_t *crys){
	H5LTread_dataset_double(file, "/mf_header/crystal/celvol", &crys->celvol);
	H5LTread_dataset_double(file, "/mf_header/crystal/recvol", &crys->recvol);
	H5LTread_dataset_double(file, "/mf_header/crystal/alat", &crys->alat);
	H5LTread_dataset_double(file, "/mf_header/crystal/blat", &crys->blat);
	H5LTread_dataset_int(file, "/mf_header/crystal/nat", &crys->nat);
	H5LTread_dataset_double(file, "/mf_header/crystal/avec", &crys->avec[0][0]);
	H5LTread_dataset_double(file, "/mf_header/crystal/bvec", &crys->bvec[0][0]);
	H5LTread_dataset_double(file, "/mf_header/crystal/adot", &crys->adot[0][0]);
	H5LTread_dataset_double(file, "/mf_header/crystal/bdot", &crys->bdot[0][0]);
	crys->atyp = (i32_t*) malloc(crys->nat*sizeof(i32_t));
	H5LTread_dataset_int(file, "/mf_header/crystal/atyp", &crys->atyp[0]);
	crys->apos = (f64_t*) malloc(crys->nat*3*sizeof(f64_t));
	H5LTread_dataset_double(file, "/mf_header/crystal/apos", &crys->apos[0]);
}


/*
 * Functions called by user
 */


void bgw_io_print_mf_header(struct mf_header_t *mf){
	printf("\n");
	printf("- Group kpoints:\n");
	printf("  - nspin (%d): ", sizeof(mf->kpoints.nspin)/sizeof(i32_t));
	printf("%d\n", mf->kpoints.nspin);
	printf("  - nspinor (%d): ", sizeof(mf->kpoints.nspinor)/sizeof(i32_t));
	printf("%d\n", mf->kpoints.nspinor);
	printf("  - nrk (%d): ", sizeof(mf->kpoints.nrk)/sizeof(i32_t));
	printf("%d\n", mf->kpoints.nrk);
	printf("  - mnband (%d): ", sizeof(mf->kpoints.mnband)/sizeof(i32_t));
	printf("%d\n", mf->kpoints.mnband);
	printf("  - ngkmax (%d): ", sizeof(mf->kpoints.ngkmax)/sizeof(i32_t));
	printf("%d\n", mf->kpoints.ngkmax);
	printf("  - ecutwfc (%d): ", sizeof(mf->kpoints.ecutwfc)/sizeof(f64_t));
	printf("%f\n", mf->kpoints.ecutwfc);
	printf("  - kgrid (%d): ", sizeof(mf->kpoints.kgrid)/sizeof(i32_t));
	printf("...\n");
	printf("  - shift (%d): ", sizeof(mf->kpoints.shift)/sizeof(f64_t));
	printf("...\n");
	printf("  - ngk (%d): ", sizeof(mf->kpoints.ngk)/sizeof(i32_t));
	printf("...\n");
	printf("  - ifmin (%d): ", sizeof(mf->kpoints.ifmin)/sizeof(i32_t));
	printf("...\n");
	printf("  - ifmax (%d): ", sizeof(mf->kpoints.ifmax)/sizeof(i32_t));
	printf("...\n");
	printf("  - w (%d): ", sizeof(mf->kpoints.w)/sizeof(f64_t));
	printf("...\n");
	printf("  - rk (%d): ", sizeof(mf->kpoints.rk)/sizeof(f64_t));
	printf("...\n");
	printf("  - el (%d): ", sizeof(mf->kpoints.el)/sizeof(f64_t));
	printf("...\n");
	printf("  - occ (%d): ", sizeof(mf->kpoints.occ)/sizeof(f64_t));
	printf("...\n");
	printf("\n");
	printf("- Group gspace:\n");
	printf("  - ng (%d): ", sizeof(mf->gspace.ng)/sizeof(i32_t));
	printf("%d\n", mf->gspace.ng);
	printf("  - ecutrho (%d): ", sizeof(mf->gspace.ecutrho)/sizeof(f64_t));
	printf("%f\n", mf->gspace.ecutrho);
	printf("  - FFTgrid (%d): ", sizeof(mf->gspace.FFTgrid)/sizeof(i32_t));
	printf("...\n");
	printf("  - components (%d): ", sizeof(mf->gspace.components)/sizeof(i32_t));
	printf("...\n");
	printf("\n");
	printf("- Group symmetry:\n");
	printf("  - ntran (%d): ", sizeof(mf->symmetry.ntran)/sizeof(i32_t));
	printf("%d\n", mf->symmetry.ntran);
	printf("  - cell_symmetry (%d): ", sizeof(mf->symmetry.cell_symmetry)/sizeof(i32_t));
	printf("%d\n", mf->symmetry.cell_symmetry);
	printf("  - mtrx (%d): ", sizeof(mf->symmetry.mtrx)/sizeof(i32_t));
	printf("...\n");
	printf("  - tnp (%d): ", sizeof(mf->symmetry.tnp)/sizeof(f64_t));
	printf("...\n");
	printf("\n");
	printf("- Group crystal:\n");
	printf("  - celvol (%d): ", sizeof(mf->crystal.celvol)/sizeof(f64_t));
	printf("%f\n", mf->crystal.celvol);
	printf("  - recvol (%d): ", sizeof(mf->crystal.recvol)/sizeof(f64_t));
	printf("%f\n", mf->crystal.recvol);
	printf("  - alat (%d): ", sizeof(mf->crystal.alat)/sizeof(f64_t));
	printf("%f\n", mf->crystal.alat);
	printf("  - blat (%d): ", sizeof(mf->crystal.blat)/sizeof(f64_t));
	printf("%f\n", mf->crystal.blat);
	printf("  - nat (%d): ", sizeof(mf->crystal.nat)/sizeof(i32_t));
	printf("%d\n", mf->crystal.nat);
	printf("  - avec (%d): ", sizeof(mf->crystal.avec)/sizeof(f64_t));
	printf("...\n");
	printf("  - bvec (%d): ", sizeof(mf->crystal.bvec)/sizeof(f64_t));
	printf("...\n");
	printf("  - adot (%d): ", sizeof(mf->crystal.adot)/sizeof(f64_t));
	printf("...\n");
	printf("  - bdot (%d): ", sizeof(mf->crystal.bdot)/sizeof(f64_t));
	printf("...\n");
	printf("  - atyp (%d): ", sizeof(mf->crystal.atyp)/sizeof(i32_t));
	printf("...\n");
	printf("  - apos (%d): ", sizeof(mf->crystal.apos)/sizeof(f64_t));
	printf("...\n");
	printf("\n");
}


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
