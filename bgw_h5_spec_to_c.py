#!/usr/bin/env python3


from bgw_h5_spec_parser import parse_h5_spec


groups_abbrev = {
    'kpoints': 'kp',
    'gspace': 'gvec',
    'symmetry': 'syms',
    'crystal': 'crys'
}


types_to_hdf5 = {
    'integer': 'int',
    'double': 'double'
}


types_to_bgw = {
    'integer': 'i32_t',
    'double': 'f64_t'
}


def populate_kws(root, kws):
    for child in root.children:
        try:
            kw = child.keywords['Dataset']
            kws[kw] = groups_abbrev[root.keywords['Group']]
        except:
            pass
        populate_kws(child, kws)


def create_write_cmd(fname_spec, group_name):
    root = parse_h5_spec(fname_spec)

    mf_header = root.get_child('mf_header')
    group = mf_header.get_child(group_name)

    kws = {}
    populate_kws(root, kws)

    lines = []
    for child in group.children:
        if child.get_type() == 'dataset':
            name = child.keywords['Dataset']
            dtype = child.keywords['Type']
            dt_hdf5 = types_to_hdf5[dtype]
            path = '/'.join(child.get_full_path())
            rank = int(child.keywords['Rank'])

            fixed_size = True
            dims = []
            for idim in range(rank):
                dim = child.keywords[f'Dims({idim+1})']
                if not dim.isdecimal():
                    fixed_size = False
                    dim = f'{kws[dim]}->{dim}'
                dims.append(dim)
            # Fortran to C order
            dims = dims[::-1]

            if rank==0:
                dims_str = '{0}'
            else:
                dims_str = '{' + ','.join(dims) + '}'

            var = f'{groups_abbrev[group_name]}->{name}'
            # If the size of the array is fixed, it is a (nested) static array
            # Otherwise we just dynamically allocate a 1D array.
            rank_allocated = rank if fixed_size else 1
            var = f'&{var}'
            if rank > 0:
                var = var + '[0]'*rank_allocated

            lines += [f'\tH5LTmake_dataset_{dt_hdf5}(file, "/{path}", {rank}, (hsize_t[]){dims_str}, {var});']

    return '\n'.join(lines)


def create_read_cmd(fname_spec, group_name):
    root = parse_h5_spec(fname_spec)

    mf_header = root.get_child('mf_header')
    group = mf_header.get_child(group_name)

    kws = {}
    populate_kws(root, kws)

    lines = []
    for child in group.children:
        if child.get_type() == 'dataset':
            name = child.keywords['Dataset']
            dtype = child.keywords['Type']
            dt_hdf5 = types_to_hdf5[dtype]
            path = '/'.join(child.get_full_path())
            rank = int(child.keywords['Rank'])

            fixed_size = True
            dims = []
            for idim in range(rank):
                dim = child.keywords[f'Dims({idim+1})']
                if not dim.isdecimal():
                    fixed_size = False
                    dim = f'{kws[dim]}->{dim}'
                dims.append(dim)
            # Fortran to C order
            dims = dims[::-1]

            var = f'{groups_abbrev[group_name]}->{name}'
            if not fixed_size:
                # Size of the array is variable, it is a pointer
                # Need to dynamically allocate variable
                dt_header = types_to_bgw[dtype]
                dims2 = dims + [f'sizeof({dt_header})']
                lines += [f'\t{var} = ({dt_header}*) malloc({"*".join(dims2)});']

            # If the size of the array is fixed, it is a (nested) static array
            # Otherwise we just dynamically allocate a 1D array.
            rank_allocated = rank if fixed_size else 1
            var = f'&{var}'
            if rank > 0:
                var = var + '[0]'*rank_allocated

            lines += [f'\tH5LTread_dataset_{dt_hdf5}(file, "/{path}", {var});']

    return '\n'.join(lines)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('fname_spec')
    parser.add_argument('group_name', choices=groups_abbrev.keys())
    args = parser.parse_args()

    #lines = main(**vars(args))
