"""Make Method

Script to write a method from a template file.

"""

import os
import textwrap
import argparse
from spp import Template, error, date
from spp.config import get_cfg

DEFAULTS = {
    'method_dir': 'methods'
}

DESC = """
generate a method file.

use the -w option to write a file of the printed output. examples:

    mkmethod gillespie ssa

    mkmethod euler sde
"""

def add_parser(subp, raw):
    """Add a parser to the main subparser. """
    tmpp = subp.add_parser('mkmethod', help='create a method file',
                            formatter_class=raw,
                            description=textwrap.dedent(DESC))
    tmpp.add_argument('method', type=str, metavar='METHODNAME',
                      help='name of the method')
    tmpp.add_argument('type', type=str, choices=['ssa', 'dsde'],
                      help='method type')
    tmpp.add_argument('-w', action="store_true", default=False,
                      help='write file')
    tmpp.add_argument('--method_dir', type=str, metavar='PATH',
                      help='directory where to write the file')


class Method(Template):
    """Structure to read a template method and make the appropiate
    substitutions. """
    def __init__(self, include):
        base = '%s/spp/methods' % include
        Template.__init__(self, '%s/base-method-template.h' % base)


def run(arg):
    """Run the command. """
    cfg = get_cfg(arg, 'mkmethod', DEFAULTS)
    root = cfg['spp']['root']
    inc = cfg['spp']['include_path']
    if inc[0] != '/':
        inc = '%s/%s' % (root, inc)
    try:
        method = Method(inc)
    except IOError:
        msg = 'ERROR: the path `%s` does contain the spp header ' \
              'files.\n' % os.path.abspath(inc)
        error(msg)
    method['USER'] = os.environ['USER']
    method['DATE'] = date()
    method['METHODNAME'] = arg.method
    method['MODELTYPE'] = arg.type

    print method

    if arg.w is True:
        mdir = cfg['mkmethod']['method_dir']
        if mdir[0] != '/':
            mdir = '%s/%s' % (root, mdir)
        fname = '%s.h' % arg.method
        print 'Writing %s to %s ...' % (fname, mdir)
        absfname = '%s/%s' % (mdir, fname)
        if os.path.exists(absfname):
            error('ERROR: file already exist\n')
        if not os.path.exists(mdir):
            error('ERROR: directory does not exist\n')
        with open(absfname, 'w') as tmpf:
            tmpf.write(str(method))
        print 'done ...'
