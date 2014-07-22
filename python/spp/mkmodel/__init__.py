"""Make Model

Script to write a model from a template file.

"""

import os
import textwrap
import argparse
from spp import Template, error, date
from spp.config import get_cfg

DEFAULTS = {
    'model_dir': 'models'
}

DESC = """
generate a model file.

use the -w option to write a file of the printed output. examples:

    mkmodel lphage ssa --num_specs 7 --num_reacs 14 -z col

    mkmodel feedforward sde --dim 2
"""


def positive_type(val):
    """Raise an error if the value is not positive. """
    val = int(val)
    if val < 1:
        raise argparse.ArgumentTypeError("positive integers only")
    return val


def add_parser(subp, raw):
    """Add a parser to the main subparser. """
    tmpp = subp.add_parser('mkmodel', help='create a model file',
                           formatter_class=raw,
                           description=textwrap.dedent(DESC))
    tmpp.add_argument('model', type=str, metavar='MODELNAME',
                      help='name of the model')
    tmpp.add_argument('type', type=str, choices=['ssa', 'dsde'],
                      help='model type')
    tmpp.add_argument('--nspecs', type=positive_type,
                      help='number of species')
    tmpp.add_argument('--nreacs', type=positive_type,
                      help='number of reactions')
    tmpp.add_argument('--dim', type=positive_type,
                      help='dimension of phase space')
    tmpp.add_argument('-z', type=str, choices=['row', 'col'],
                      help='state change matrix format')
    tmpp.add_argument('-w', action="store_true", default=False,
                      help='write file')
    tmpp.add_argument('--model_dir', type=str, metavar='PATH',
                      help='directory where to write the file')


class Model(Template):
    """Structure to read a template model and make the appropiate
    substitutions. """
    def __init__(self, include, mtype='ssa'):
        base = '%s/spp/models' % include
        Template.__init__(self, '%s/model-%s-template.h' % (base, mtype))


def _ssa_model(arg, model):
    """Write strings related to ssa. """
    numspecs = arg.nspecs
    numreacs = arg.nreacs

    prop = ['prop[0] = 0;']
    for num in xrange(1, numreacs):
        prop.append('        prop[%d] = 0;' % num)
    model['PROP'] = '\n'.join(prop)

    model['Z'] = 'for(int i=0; i < z.size(1); ++i) z[i] = 0;'
    if arg.z:
        model['Z'] += '\n'
        sfmt = '%%%dd' % len(str(numspecs-1))
        rfmt = '%%%dd' % len(str(numreacs-1))
        if arg.z == 'row':
            for reac in xrange(0, numreacs):
                tmp = ' '*6
                for spec in xrange(0, numspecs):
                    tmp += ('  z(%s, %s) =  0;' % (sfmt, rfmt)) % (spec, reac)
                model['Z'] += '%s\n' % tmp
        else:
            for spec in xrange(0, numspecs):
                tmp = ' '*6
                for reac in xrange(0, numreacs):
                    tmp += ('  z(%s, %s) =  0;' % (sfmt, rfmt)) % (spec, reac)
                model['Z'] += '%s\n' % tmp


def run(arg):
    """Run the command. """
    cfg = get_cfg(arg, 'mkmodel', DEFAULTS)
    root = cfg['spp']['root']
    inc = cfg['spp']['include_path']
    if inc[0] != '/':
        inc = '%s/%s' % (root, inc)
    try:
        model = Model(inc, arg.type)
    except IOError:
        msg = 'ERROR: the path `%s` does contain the spp header ' \
              'files.\n' % os.path.abspath(inc)
        error(msg)
    model['USER'] = os.environ['USER']
    model['DATE'] = date()
    model['MODELNAME'] = str(arg.model)
    model['NUMSPECS'] = str(arg.nspecs)
    model['NUMREACS'] = str(arg.nreacs)
    model['DIM'] = str(arg.dim)

    if arg.type == 'ssa':
        if arg.nspecs is None or arg.nreacs is None:
            error("ERROR: --n[spe/rea]cs is required with an ssa model\n")
        numspecs = arg.nspecs
        _ssa_model(arg, model)
    else:
        numspecs = arg.dim
        if arg.dim is None:
            error("ERROR: --dim is required with a dsde model\n")

    x_0 = ['x0[0] = 0;']
    for num in xrange(1, numspecs):
        x_0.append('        x0[%d] = 0;' % num)
    model['X0'] = '\n'.join(x_0)

    print model

    if arg.w is True:
        mdir = cfg['mkmodel']['model_dir']
        if mdir[0] != '/':
            mdir = '%s/%s' % (root, mdir)
        fname = '%s_%s.h' % (arg.model.lower(), arg.type)
        print 'Writing %s to %s ...' % (fname, mdir)
        absfname = '%s/%s' % (mdir, fname)
        if os.path.exists(absfname):
            error('ERROR: file already exist\n')
        if not os.path.exists(mdir):
            error('ERROR: directory does not exist\n')
        with open(absfname, 'w') as tmpf:
            tmpf.write(str(model))
        print 'done ...'
