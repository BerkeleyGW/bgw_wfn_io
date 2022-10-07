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


void setup_wfns(hid_t file, struct mf_header_t *mf){
	if (!H5Lexists(file, "/wfns", H5P_DEFAULT)) {
		create_group(file, "/wfns");
	}

	hsize_t ngktot = 0;
	for (int ik=0; ik<mf->kpoints.nrk; ++ik) ngktot += mf->kpoints.ngk[ik];

	// Create /wfns/gvecs dataset
	if (H5Lexists(file, "/wfns/gvecs", H5P_DEFAULT)) {
		H5Ldelete(file, "/wfns/gvecs", H5P_DEFAULT);
	}
	hsize_t dims_g[] = {ngktot, 3};
	hid_t dspace_g = H5Screate_simple(2, dims_g, NULL);
	hid_t dset_g = H5Dcreate2(file, "/wfns/gvecs", H5T_STD_I32LE, dspace_g, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Dclose(dset_g);
	H5Sclose(dspace_g);

	if (H5Lexists(file, "/wfns/coeffs", H5P_DEFAULT)) {
		H5Ldelete(file, "/wfns/coeffs", H5P_DEFAULT);
	}
	// Create /wfns/coeffs dataset
	hsize_t ns = (hsize_t) mf->kpoints.nspin * mf->kpoints.nspinor;
	hsize_t dims_c[] = {(hsize_t)mf->kpoints.mnband, ns, ngktot, 2};
	hid_t dspace_c = H5Screate_simple(4, dims_c, NULL);
	hid_t dset_c = H5Dcreate2(file, "/wfns/coeffs", H5T_IEEE_F64LE, dspace_c, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Dclose(dset_c);
	H5Sclose(dspace_c);
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


/** Free mf_header structure after use by bgw_io_read_mf_header
 *
 *  @param mf Mean-field structure
 */
void bgw_io_free_mf_header(struct mf_header_t *mf) {
	free(mf->kpoints.ngk);
	free(mf->kpoints.ifmin);
	free(mf->kpoints.ifmax);
	free(mf->kpoints.w);
	free(mf->kpoints.rk);
	free(mf->kpoints.el);
	free(mf->kpoints.occ);
	free(mf->gspace.components);
	free(mf->crystal.atyp);
	free(mf->crystal.apos);
}


/** Setups the internal structure for an output wavefunction file.
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Output mean-field structure
 *  @param do_setup_wfns Whether to also setup the structure for the
 *         wavefunction coefficients (=1) or only setup the mean-field
 *         structure (=0).
 */
void bgw_io_setup_wfn(const char *fname, struct mf_header_t *mf, int do_setup_wfns){
	hid_t file;

	file = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	setup_mf_header(file);
	if (do_setup_wfns) setup_wfns(file, mf);
	H5Fclose(file);
}


/** Write the header (/mf_header) to a wavefunction file.
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Output mean-field structure
 */
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


/** Read the header (/mf_header) from a wavefunction file.
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Output mean-field structure
 */
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


/** Read G-vectors {G}, in crystal coordinates, associated with the
 *  wavefunctions at k-point rk[ik].
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Input/output mean-field structure
 *  @param ik Index of the k-point to read. Actual k-point is mf->kpoints.rk[ik]
 *  @param gvecs Input/output buffer. Must be at least ngk_ik*3 large,
 *         where ngk_ik = mf->kpoints.ngk[ik]. Output data is organized as
 *         (ig,j), where ig is the G-vector index and j is the direction along
 *         b_1, b_2 or b_3 reciprocal lattice vectors.
 */
void bgw_io_read_gvecs_ik_serial(const char *fname, struct mf_header_t *mf, int ik, i32_t *gvecs){

	int ng_offset = 0;
	const int ng_ik = mf->kpoints.ngk[ik];

	for (int ik_=0; ik_<ik; ++ik_) ng_offset += mf->kpoints.ngk[ik_];

	hid_t file = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);

	// G-vectors associated with each k-point
	hid_t dset_g = H5Dopen2(file, "/wfns/gvecs", H5P_DEFAULT);
	// Pointer to the memory region in the file
	hid_t dspace_g = H5Dget_space(dset_g);

	// How much to offset the memory we are writing to file?
	hsize_t offset_g[2] = {(hsize_t)ng_offset, 0};
	// How many entries to write?
	hsize_t count_g[2] = {(hsize_t)ng_ik, 3};

	// Pointer to the memory region in the buffer
	hid_t memspace_g = H5Screate_simple(2, count_g, NULL);
	// Select the section (hyperslab) of the gvecs dataset to write
	H5Sselect_hyperslab(dspace_g, H5S_SELECT_SET, offset_g, NULL, count_g, NULL);
	H5Dread(dset_g, H5T_STD_I32LE, memspace_g, dspace_g, H5P_DEFAULT, gvecs);

	// Always close whatever you open/create
	H5Sclose(memspace_g);
	H5Sclose(dspace_g);
	H5Dclose(dset_g);
	H5Fclose(file);
}


/** Write G-vectors {G}, in crystal coordinates, associated with the
 *  wavefunctions at k-point rk[ik].
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Input/output mean-field structure
 *  @param ik Index of the k-point to write. Actual k-point is mf->kpoints.rk[ik]
 *  @param gvecs Input/output buffer. Must be at least ngk_ik*3 large,
 *         where ngk_ik = mf->kpoints.ngk[ik]. Output data is organized as
 *         (ig,j), where ig is the G-vector index and j is the direction along
 *         b_1, b_2 or b_3 reciprocal lattice vectors.
 */
void bgw_io_write_gvecs_ik_serial(const char *fname, struct mf_header_t *mf, int ik, i32_t *gvecs){

	int ng_offset = 0;
	const int ng_ik = mf->kpoints.ngk[ik];

	for (int ik_=0; ik_<ik; ++ik_) ng_offset += mf->kpoints.ngk[ik_];

	hid_t file = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);

	// G-vectors associated with each k-point
	hid_t dset_g = H5Dopen2(file, "/wfns/gvecs", H5P_DEFAULT);
	// Pointer to the memory region in the file
	hid_t dspace_g = H5Dget_space(dset_g);

	// How much to offset the memory we are writing to file?
	hsize_t offset_g[2] = {(hsize_t)ng_offset, 0};
	// How many entries to write?
	hsize_t count_g[2] = {(hsize_t)ng_ik, 3};

	// Pointer to the memory region in the buffer
	hid_t memspace_g = H5Screate_simple(2, count_g, NULL);
	// Select the section (hyperslab) of the gvecs dataset to write
	H5Sselect_hyperslab(dspace_g, H5S_SELECT_SET, offset_g, NULL, count_g, NULL);
	H5Dwrite(dset_g, H5T_STD_I32LE, memspace_g, dspace_g, H5P_DEFAULT, gvecs);

	// Always close whatever you open/create
	H5Sclose(memspace_g);
	H5Sclose(dspace_g);
	H5Dclose(dset_g);
	H5Fclose(file);
}


/** Read wavefunction coefficients c_nk{G} at k-point rk[ik].
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Input/output mean-field structure
 *  @param ik Index of the k-point to read. Actual k-point is mf->kpoints.rk[ik]
 *  @param coeffs Input/output buffer. Must be at least nbngk_ik*3 large,
 *         where ngk_ik = mf->kpoints.ngk[ik]. Output data is organized as
 *         (ib,is,ig,2), where ib is the band index, is is the spin index,
 *         ig is the G-vector index, and 2 represents real/complex parts.
 */
void bgw_io_read_coeffs_ik_serial(const char *fname, struct mf_header_t *mf, int ik, f64_t *coeffs){

	int ng_offset = 0;
	const int ng_ik = mf->kpoints.ngk[ik];
	const int nb = mf->kpoints.mnband;
	const int ns = mf->kpoints.nspin * mf->kpoints.nspinor;

	for (int ik_=0; ik_<ik; ++ik_) ng_offset += mf->kpoints.ngk[ik_];

	hid_t file = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);

	// G-vectors associated with each k-point
	hid_t dset_c = H5Dopen2(file, "/wfns/coeffs", H5P_DEFAULT);
	// Pointer to the memory region in the file
	hid_t dspace_c = H5Dget_space(dset_c);

        // How much to offset the memory we are reading from file?
        hsize_t offset_c[4] = {0, 0, (hsize_t)ng_offset, 0};
        // How many entries to read?
        hsize_t count_c[4] = {(hsize_t)nb, (hsize_t)ns, (hsize_t)ng_ik, 2};

        // Pointer to the memory region in the buffer
        hid_t memspace_c = H5Screate_simple(4, count_c, NULL);
        // Select the section (hyperslab) of the coeffs dataset to read
        H5Sselect_hyperslab(dspace_c, H5S_SELECT_SET, offset_c, NULL, count_c, NULL);
        H5Dread(dset_c, H5T_IEEE_F64LE, memspace_c, dspace_c, H5P_DEFAULT, coeffs);

	// Always close whatever you open/create
        H5Sclose(memspace_c);
	H5Sclose(dspace_c);
	H5Dclose(dset_c);
	H5Fclose(file);
}


/** Write wavefunction coefficients c_nk{G} at k-point rk[ik].
 *
 *  @param fname Wavefunction file name (hdf5)
 *  @param mf Input/output mean-field structure
 *  @param ik Index of the k-point to write. Actual k-point is mf->kpoints.rk[ik]
 *  @param coeffs Input/output buffer. Must be at least nbngk_ik*3 large,
 *         where ngk_ik = mf->kpoints.ngk[ik]. Output data is organized as
 *         (ib,is,ig,2), where ib is the band index, is is the spin index,
 *         ig is the G-vector index, and 2 represents real/complex parts.
 */
void bgw_io_write_coeffs_ik_serial(const char *fname, struct mf_header_t *mf, int ik, f64_t *coeffs){

	int ng_offset = 0;
	const int ng_ik = mf->kpoints.ngk[ik];
	const int nb = mf->kpoints.mnband;
	const int ns = mf->kpoints.nspin * mf->kpoints.nspinor;

	for (int ik_=0; ik_<ik; ++ik_) ng_offset += mf->kpoints.ngk[ik_];

	hid_t file = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);

	// G-vectors associated with each k-point
	hid_t dset_c = H5Dopen2(file, "/wfns/coeffs", H5P_DEFAULT);
	// Pointer to the memory region in the file
	hid_t dspace_c = H5Dget_space(dset_c);

        // How much to offset the memory we are reading from file?
        hsize_t offset_c[4] = {0, 0, (hsize_t)ng_offset, 0};
        // How many entries to read?
        hsize_t count_c[4] = {(hsize_t)nb, (hsize_t)ns, (hsize_t)ng_ik, 2};

        // Pointer to the memory region in the buffer
        hid_t memspace_c = H5Screate_simple(4, count_c, NULL);
        // Select the section (hyperslab) of the coeffs dataset to read
        H5Sselect_hyperslab(dspace_c, H5S_SELECT_SET, offset_c, NULL, count_c, NULL);
        H5Dwrite(dset_c, H5T_IEEE_F64LE, memspace_c, dspace_c, H5P_DEFAULT, coeffs);

	// Always close whatever you open/create
        H5Sclose(memspace_c);
	H5Sclose(dspace_c);
	H5Dclose(dset_c);
	H5Fclose(file);
}


