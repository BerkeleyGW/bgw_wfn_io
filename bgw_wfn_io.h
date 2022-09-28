/* -*- indent-tabs-mode: t -*- */

#ifndef BGW_WFN_IO_H
#define BGW_WFN_IO_H

#include <stdint.h>

/*
 Copyright (C) 2022 Felipe H. da Jornada
*/

#define BGW_WFN_VERSION 1

typedef int32_t i32_t;
typedef double f64_t;


struct kpoints_t {
	i32_t nspin;       // nspin = 1 or 2; nspin = 1 when npsinor = 2
	i32_t nspinor;     // nspinor = 2 if doing two-component spinor calculation; 1 is default
	i32_t nrk;         // number of k-points
	i32_t mnband;      // max number of bands
	i32_t ngkmax;      // max. number of G-vectors per kpoint = max(ngk(:))
	f64_t ecutwfc;  // wave-function cutoff, in Ry
	i32_t kgrid[3];    // Monkhorst-Pack number of k-points in each direction
	f64_t shift[3]; // Monkhorst-Pack shift of grid
	i32_t *ngk;    // number of g-vectors for each k-point
	i32_t *ifmin;  // lowest occupied band (kpoint,spin)
	i32_t *ifmax;  // highest occupied band (kpoint,spin)
	f64_t *w;   // weights (kpoint) (between 0 and 1)
	f64_t *rk;  // k-vector (3, kpoint) in crystal coords
	f64_t *el;  // band energies (band, kpoint, spin)
	f64_t *occ; // occupations (between 0 and 1)
};

struct gspace_t {
	i32_t ng;          // number of G-vectors
	f64_t ecutrho;  // charge-density cutoff, in Ry
	i32_t FFTgrid[3];  // gsm: FFTgrid is the size of the FFT grid, not the maximum G-vector   
	i32_t *components; // the G-vectors (3,ng)^T, in units of b_i
};

struct symmetry_t {
	i32_t ntran;           // number of operations in full group
	i32_t cell_symmetry;   // 0=cubic, 1=hexagonal
	i32_t mtrx[48][3][3];  // symmetry matrix
	f64_t tnp[48][3];   // fractional translations
};

struct crystal_t {
	f64_t celvol; // cell volume in real space (a.u.)
	f64_t recvol; // cell volume in reciprocal space (a.u.)
	f64_t alat;   // lattice constant in real space (a.u.)
	f64_t blat;   // lattice constant in reciprocal space (a.u.)
	f64_t avec[3][3]; // lattice vectors in real space (alat) (3x3)
	f64_t bvec[3][3]; // lattice vectors in reciprocal space (blat) (3x3)
	f64_t adot[3][3]; // metric tensor in real space (a.u.) (3x3)
	f64_t bdot[3][3]; // metric tensor in reciprocal space (a.u.) (3x3)
	i32_t nat;      // number of atoms
	i32_t *atyp;    // atomic species, atyp(1:nat)
	f64_t *apos; // atomic positions, apos(1:3,1:nat)^T (alat)
};

struct mf_header_t {
	i32_t versionnumber;
	i32_t flavor;
	struct kpoints_t kpoints;
	struct gspace_t gspace;
	struct symmetry_t symmetry;
	struct crystal_t crystal;
};

#ifdef __cplusplus
extern "C" {
#endif

void bgw_io_setup_wfn(const char *fname);
void bgw_io_print_mf_header(struct mf_header_t *mf);
void bgw_io_read_mf_header(const char *fname, struct mf_header_t *mf);
void bgw_io_write_mf_header(const char *fname, struct mf_header_t *mf);
//void bgw_io_write_mf_gvecs(const char *fname, struct mf_header_t *mf);
//void bgw_io_write_mf_coeffs(const char *fname, struct mf_header_t *mf);

#ifdef __cplusplus
}
#endif

#endif
