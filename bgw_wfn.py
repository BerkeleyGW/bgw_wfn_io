
from ctypes import Structure, c_int32, c_double, POINTER


class KPOINTS(Structure):
    _fields_ = [
        ('nspin', c_int32),
	('nspinor', c_int32),
	('nrk', c_int32),
	('mnband', c_int32),
	('ngkmax', c_int32),
	('ecutwfc', c_double),
	('kgrid', c_int32*3),
	('shift', c_int32*3),
	('ngk', POINTER(c_int32)),
	('ifmin', POINTER(c_int32)),
	('ifmax', POINTER(c_int32)),
        ('w', POINTER(c_double)),
        ('rk', POINTER(c_double)),
        ('el', POINTER(c_double)),
        ('occ', POINTER(c_double))
    ]


class GSPACE(Structure):
    _fields_ = [
        ('ng', c_int32),
        ('ecutrho', c_double),
        ('FFTgrid', c_int32*3),
        ('components', POINTER(c_int32))
    ]


class SYMMETRY(Structure):
    _fields_ = [
        ('ntran', c_int32),
        ('cell_symmetry', c_int32),
        ('mtrx', c_int32*48*3*3),
        ('tnp', c_double*48*3)
    ]


class CRYSTAL(Structure):
    _fields_ = [
        ('celvol', c_double),
        ('recvol', c_double),
        ('alat', c_double),
        ('blat', c_double),
        ('avec', c_double*9),
        ('bvec', c_double*9),
        ('adot', c_double*9),
        ('bdot', c_double*9),
        ('nat', c_int32),
        ('atyp', POINTER(c_int32)),
        ('apos', POINTER(c_double))
    ]


class MF_HEADER(Structure):
    _fields_ = [
        ('kpoints_t', KPOINTS),
        ('gspace_t', GSPACE),
        ('symmetry_t', SYMMETRY),
        ('crystal_t', CRYSTAL),
    ]
