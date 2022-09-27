/* -*- indent-tabs-mode: t -*- */

#ifndef BGW_WFN_IO_H
#define BGW_WFN_IO_H

/*
 Copyright (C) 2022 Felipe H. da Jornada
*/


struct kpoints_t {
	int nspin;       // nspin = 1 or 2; nspin = 1 when npsinor = 2
	int nspinor;     // nspinor = 2 if doing two-component spinor calculation; 1 is default
	int nrk;         // number of k-points
	int mnband;      // max number of bands
	int ngkmax;      // max. number of G-vectors per kpoint = max(ngk(:))
	double ecutwfc;  // wave-function cutoff, in Ry
	int kgrid[3];    // Monkhorst-Pack number of k-points in each direction
	double shift[3]; // Monkhorst-Pack shift of grid
	int *ngk;    // number of g-vectors for each k-point
	int *ifmin;  // lowest occupied band (kpoint,spin)
	int *ifmax;  // highest occupied band (kpoint,spin)
	double *w;   // weights (kpoint) (between 0 and 1)
	double *rk;  // k-vector (3, kpoint) in crystal coords
	double *el;  // band energies (band, kpoint, spin)
	double *occ; // occupations (between 0 and 1)
};

struct gspace_t {
	int ng;          // number of G-vectors
	double ecutrho;  // charge-density cutoff, in Ry
	int FFTgrid[3];  // gsm: FFTgrid is the size of the FFT grid, not the maximum G-vector   
	int *components; // the G-vectors (3,ng)^T, in units of b_i
};

struct symmetry_t {
	int ntran;           // number of operations in full group
	int cell_symmetry;   // 0=cubic, 1=hexagonal
	int mtrx[48][3][3];  // symmetry matrix
	double tnp[48][3];   // fractional translations
};

struct crystal_t {
	double celvol; // cell volume in real space (a.u.)
	double recvol; // cell volume in reciprocal space (a.u.)
	double alat;   // lattice constant in real space (a.u.)
	double blat;   // lattice constant in reciprocal space (a.u.)
	double avec[3][3]; // lattice vectors in real space (alat) (3x3)
	double bvec[3][3]; // lattice vectors in reciprocal space (blat) (3x3)
	double adot[3][3]; // metric tensor in real space (a.u.) (3x3)
	double bdot[3][3]; // metric tensor in reciprocal space (a.u.) (3x3)
	int nat;      // number of atoms
	int *atyp;    // atomic species, atyp(1:nat)
	double *apos; // atomic positions, apos(1:3,1:nat)^T (alat)
};

struct mf_header_t {
	struct kpoints_t kpoints;
	struct gspace_t gspace;
	struct symmetry_t symmetry;
	struct crystal_t crystal;
};

#ifdef __cplusplus
extern "C" {
#endif

void bgw_io_write_mf_header(const char *fname, struct mf_header_t mf_header);
void bgw_io_write_mf_gvecs(const char *fname, struct mf_header_t);
void bgw_io_write_mf_coeffs(const char *fname, struct mf_header_t);

#ifdef __cplusplus
}
#endif

#endif
