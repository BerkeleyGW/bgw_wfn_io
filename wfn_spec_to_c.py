#!/usr/bin/env python3


from spec_parser import parse_h5_spec


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


types_to_fmt = {
    'integer': '%d',
    'double': '%f'
}


def populate_kws(root, kws):
    for child in root.children:
        if child.type == 'dataset':
            # Only include datasets of kpoints, gspace, symmetry, and crystal
            if child.parent.name in groups_abbrev.keys():
                kws[child.name] = groups_abbrev[child.parent.name]
        populate_kws(child, kws)


def _create_io_cmd(fname_spec, group_name, io):
    # io is `write` or `read`
    root = parse_h5_spec(fname_spec)

    mf_header = root.get_child('mf_header')
    group = mf_header.get_child(group_name)

    kws = {}
    populate_kws(mf_header, kws)

    lines = []
    for child in group.children:
        if child.type == 'dataset':
            name = child.name
            dtype = child.dtype
            dt_hdf5 = types_to_hdf5[dtype]
            path = '/'.join(child.get_full_path())
            rank = child.rank

            fixed_size = True
            dims = []
            for idim in range(rank):
                dim = child.dims[idim]
                if not dim.isdecimal():
                    fixed_size = False
                    dim = f'{kws[dim]}->{dim}'
                dims.append(dim)
            # Fortran to C order
            dims = dims[::-1]

            # Input/output variable we will write from/read to
            var = f'{groups_abbrev[group_name]}->{name}'

            # Need to pass variables to HDF5 by reference.
            # If the size of the array is fixed, it is a (nested) static array.
            # Otherwise it is a dynamically allocated 1D array.
            rank_allocated = rank if fixed_size else 1
            var_hdf5 = f'&{var}'
            if rank > 0:
                var_hdf5 = var_hdf5 + '[0]'*rank_allocated

            if io=='write':

                if rank==0:
                    dims_str = '{0}'
                else:
                    dims_str = '{' + ','.join(dims) + '}'
                lines += [f'\tH5LTmake_dataset_{dt_hdf5}(file, "/{path}", {rank}, (hsize_t[]){dims_str}, {var_hdf5});']

            elif io=='read':

                if not fixed_size:
                    # Need to dynamically allocate pointer array
                    dt_header = types_to_bgw[dtype]
                    dims2 = dims + [f'sizeof({dt_header})']
                    lines += [f'\t{var} = ({dt_header}*) malloc({"*".join(dims2)});']
                lines += [f'\tH5LTread_dataset_{dt_hdf5}(file, "/{path}", {var_hdf5});']

            elif io=='free':
                if not fixed_size:
                    # Free array that was dynamically allocated
                    lines += [f'\tfree(mf->{group_name}.{name});']

            else:
                raise ValueError(f'{io=}')

    return '\n'.join(lines)


def create_read_cmd(fname_spec, group_name):
    return _create_io_cmd(fname_spec, group_name, 'read')


def create_write_cmd(fname_spec, group_name):
    return _create_io_cmd(fname_spec, group_name, 'write')

def create_free_cmd(fname_spec, group_name):
    return _create_io_cmd(fname_spec, group_name, 'free')


def create_print_cmd(fname_spec):
    root = parse_h5_spec(fname_spec)

    mf_header = root.get_child('mf_header')

    lines = []
    for node in mf_header:
        lvl = node.level
        name = node.name
        if node.type == 'group':
            lines.append(f'\tprintf("\\n");')
            lines.append(f'\tprintf("- Group {name}:\\n");')
        elif node.type == 'dataset':
            pname = node.parent.name
            if pname == 'mf_header':
                continue
            dtype = node.dtype
            dt_header = types_to_bgw[dtype]
            dt_fmt = types_to_fmt[dtype]
            lines.append(f'\tprintf("  - {name} (%d): ", sizeof(mf->{pname}.{name})/sizeof({dt_header}));')
            if node.rank == 0:
                lines.append(f'\tprintf("{dt_fmt}\\n", mf->{pname}.{name});')
            else:
                lines.append(f'\tprintf("...\\n");')
    lines.append(f'\tprintf("\\n");')

    return '\n'.join(lines)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('fname_spec')
    parser.add_argument('group_name', choices=groups_abbrev.keys())
    args = parser.parse_args()

    #lines = main(**vars(args))
