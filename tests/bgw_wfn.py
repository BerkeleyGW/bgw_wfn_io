
from ctypes import Structure, c_int32, c_double, POINTER

i32_t = c_int32
f64_t = c_double


class KPOINTS(Structure):
    _fields_ = [
        ('nspin', i32_t),
	('nspinor', i32_t),
	('nrk', i32_t),
	('mnband', i32_t),
	('ngkmax', i32_t),
	('ecutwfc', f64_t),
	('kgrid', i32_t*3),
	('shift', f64_t*3),
	('ngk', POINTER(i32_t)),
	('ifmin', POINTER(i32_t)),
	('ifmax', POINTER(i32_t)),
        ('w', POINTER(f64_t)),
        ('rk', POINTER(f64_t)),
        ('el', POINTER(f64_t)),
        ('occ', POINTER(f64_t))
    ]


class GSPACE(Structure):
    _fields_ = [
        ('ng', i32_t),
        ('ecutrho', f64_t),
        ('FFTgrid', i32_t*3),
        ('components', POINTER(i32_t))
    ]


class SYMMETRY(Structure):
    _fields_ = [
        ('ntran', i32_t),
        ('cell_symmetry', i32_t),
        ('mtrx', i32_t*48*3*3),
        ('tnp', f64_t*48*3)
    ]


class CRYSTAL(Structure):
    _fields_ = [
        ('celvol', f64_t),
        ('recvol', f64_t),
        ('alat', f64_t),
        ('blat', f64_t),
        ('avec', f64_t*9),
        ('bvec', f64_t*9),
        ('adot', f64_t*9),
        ('bdot', f64_t*9),
        ('nat', i32_t),
        ('atyp', POINTER(i32_t)),
        ('apos', POINTER(f64_t))
    ]


class MF_HEADER(Structure):
    _fields_ = [
        ('versionnumber', i32_t),
        ('flavor', i32_t),
        ('kpoints', KPOINTS),
        ('gspace', GSPACE),
        ('symmetry', SYMMETRY),
        ('crystal', CRYSTAL),
    ]
