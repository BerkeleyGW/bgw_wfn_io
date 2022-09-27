#!/usr/bin/env python3


from bgw_h5_spec_parser import parse_h5_spec


groups_abbrev = {
    'kpoints': 'kp',
    'gspace': 'gvec',
    'symmetry': 'syms',
    'crystal': 'crys'
}


types_to_c = {
    'integer': 'int',
    'double': 'double'
}


def populate_kws(root, kws):
    for child in root.children:
        try:
            kw = child.keywords['Dataset']
            kws[kw] = groups_abbrev[root.keywords['Group']]
        except:
            pass
        populate_kws(child, kws)


def bgw_h5_spec_to_c(fname_spec, group_name):
    root = parse_h5_spec(fname_spec)

    mf_header = root.get_child('mf_header')
    group = mf_header.get_child(group_name)

    kws = {}
    populate_kws(root, kws)

    lines = []
    for child in group.children:
        if child.get_type() == 'dataset':
            name = child.keywords['Dataset']
            dt = types_to_c[child.keywords['Type']]
            path = '/'.join(child.get_full_path())
            rank = int(child.keywords['Rank'])

            use_ampersand = True
            dims = []
            for idim in range(rank):
                dim = child.keywords[f'Dims({idim+1})']
                if not dim.isdecimal():
                    use_ampersand = False
                    dim = f'{kws[dim]}.{dim}'
                dims.append(dim)
            # Fortran to C order
            dims = dims[::-1]

            if rank==0:
                dims = '{0}'
            else:
                dims = '{' + ','.join(dims) + '}'

            var = f'{groups_abbrev[group_name]}.{name}'
            if use_ampersand:
                var = f'&{var}'
                if rank > 0:
                    var = var + '[0]'*rank
            else:
                var = f'{var}'

            lines += [f'\tH5LTmake_dataset_{dt}(file, "/{path}", {rank}, (hsize_t[]){dims}, {var});']

    return lines


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('fname_spec')
    parser.add_argument('group_name', choices=groups_abbrev.keys())
    args = parser.parse_args()

    lines = main(**vars(args))
    print('\n'.join(lines))
