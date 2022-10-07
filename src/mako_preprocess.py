#!/usr/bin/env python3

# Recursively preprocess all .cp files, using Mako template engine, and write
# the corresponding .c processed files. You will need to manually run this
# script after changing any .cp file, and commit both the input .cp and
# output .c files.
#
# Instructions:
# 1) Learn Mako template (http://www.makotemplates.org/).
# 2) Install Mako with `pip install --user Mako`.
# 3) Create your awesome source .cp using mako templates
#   (see Common/hdf5_io.cp for an example)
# 4) Run bin/mako_preprocess.py to preprocess all source Mako templates.
#    IMPORTANT: our make system will never do this automatically, because
#    we don't want to force the user to install Mako.
# 5) When happy with the result, commit both the input (.cp) and output
#    (.c) files.
#
# Felipe H. da Jornada (Mar 2017)


from __future__ import print_function
from mako.template import Template
from mako import exceptions
import fnmatch
import sys
import os


def gen_mako_signature(fname_in, fname_out):
    basename = os.path.basename(sys.argv[0])
    return 'File {} automatically created from {} by {}.'.format(fname_out,
        fname_in, basename)


def get_files(root_dir='.', fn_filter='*.cp', ignore_patterns=None):
    if ignore_patterns is None:
        ignore_patterns = ['.git', '.svn']

    matches = []
    for root, dirnames, filenames in os.walk(root_dir):
        for pattern in ignore_patterns:
            if pattern in root:
                break
        else:
            for filename in fnmatch.filter(filenames, '*.cp'):
                fname = os.path.join(root, filename)
                matches.append(fname)
    return matches


def main(files=None):
    if files is None or len(files)==0:
        files = get_files()

    for fname in files:
        fname_out = fname.rsplit('.cp')[0] + '.c'
        print('{} -> {}'.format(fname, fname_out))
        mako_signature = gen_mako_signature(os.path.basename(fname),
            os.path.basename(fname_out))
        dont_edit = 'Do not edit the resulting file ({}) directly!'.format(
            os.path.basename(fname_out))

        #template = Template(filename=fname)
        with open(fname, 'r') as f:
            data = f.read()
            template = Template(text=data)
        _vars = dict(mako_signature=mako_signature, dont_edit=dont_edit, source_fname=fname)
        try:
            rendered_str = template.render(**_vars)
        except:
            print(exceptions.text_error_template().render())
            raise

        with open(fname_out, 'w') as f:
            f.write(rendered_str)


if __name__ == '__main__':
    import argparse
    desc = ('Mako preprocess cp files. Default is to preprocess all *.cp '
            'files that can be found under the current directory and its '
            'subdirectories.')
    parser = argparse.ArgumentParser(description=desc)
    parser.add_argument('files', nargs='*', help=
                        'Only preprocesses FILES instead of all *.cp under '
                        'current directory/subdirectories.')
    args = parser.parse_args()
    main(**vars(args))
