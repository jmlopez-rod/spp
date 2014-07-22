"""Make Collector

Script to write a data collector from a template file.

"""

import os
import textwrap
import argparse
from spp import Template, error, date
from spp.config import get_cfg

DEFAULTS = {
    'collector_dir': 'collectors'
}

DESC = """
generate a data collector file.

use the -w option to write a file of the printed output.

"""


def add_parser(subp, raw):
    """Add a parser to the main subparser. """
    tmpp = subp.add_parser('mkcollector', help='create a collector file',
                           formatter_class=raw,
                           description=textwrap.dedent(DESC))
    tmpp.add_argument('collector', type=str, metavar='COLLECTORNAME',
                      help='name of the collector')
    tmpp.add_argument('type', type=str, choices=['ssa', 'dsde'],
                      help='model type')
    tmpp.add_argument('--method', type=str,
                      default='base_method', help='method name')
    tmpp.add_argument('--model', type=str,
                      default='', help='model name')
    tmpp.add_argument('-w', action="store_true", default=False,
                      help='write file')
    tmpp.add_argument('--collector_dir', type=str, metavar='PATH',
                      help='directory where to write the file')


class Collector(Template):
    """Structure to read a collector template and make the appropiate
    substitutions. """
    def __init__(self, include):
        base = '%s/spp/collectors' % include
        Template.__init__(self, '%s/collector-template.h' % base)


def _get_properties(filename):
    """Returns the text found between `//@begin-properties` and
    `//@end-properties`"""
    with open(filename, 'r') as tmpf:
        txt = tmpf.read()
    index = txt.find('//@begin-properties')
    prop = ''
    while index > -1:
        endindex = txt.find('//@end-properties', index)
        if endindex == -1:
            msg = "ERROR: Unable to find '//@end-properties'\n" \
                  " file: %s\n //@begin-properties starts at line " \
                  "%d\n" % (filename, txt.count('\n', 0, index)+1)
            error(msg)
        else:
            prop += txt[index+len('//@begin-properties')+1:endindex-1]
            index = txt.find('//@begin-properties', endindex)
    prop = prop.split('\n')
    for i in xrange(0, len(prop)):
        prop[i] = prop[i].strip()
    return '\n        '.join(prop)


def _write_method_props(collector, cfg_method, inc):
    """Helper function to write the method properties. """
    fname = '%s/spp/methods/base_method.h' % inc
    collector['STDMETHODPROP'] = _get_properties(fname)
    if collector['METHODNAME'] != 'base_method':
        found = False
        for tmpr in [cfg_method['method_dir'], inc]:
            fname = '%s/methods/%s.h' % (tmpr, collector['METHODNAME'])
            try:
                collector['METHODPROP'] = _get_properties(fname)
                found = True
                break
            except IOError:
                pass
        if not found:
            msg = 'ERROR: Unable to find %s\n' % collector['METHODNAME']
            error(msg)
    else:
        collector['METHODPROP'] = ''


def _write_model_props(arg, collector, cfg_model, inc):
    """Helper function to write the model properties. """
    fname = '%s/spp/models/model_%s.h' % (inc, arg.type)
    collector['STDMODELPROP'] = _get_properties(fname)
    if arg.model != '':
        found = False
        for tmpr in [cfg_model['model_dir'], inc]:
            fname = '%s/models/%s.h' % (tmpr, collector['MODELNAME'])
            try:
                collector['MODELPROP'] = _get_properties(fname)
                found = True
                break
            except IOError:
                pass
        if not found:
            msg = 'ERROR: Unable to find %s\n' % collector['MODELNAME']
            error(msg)
    else:
        collector['MODELPROP'] = ''


def run(arg):
    """Run the command. """
    cfg = get_cfg(arg, ['mkcollector', 'mkmethod', 'mkmodel'])
    root = cfg['spp']['root']
    inc = cfg['spp']['include_path']
    if inc[0] != '/':
        inc = '%s/%s' % (root, inc)
    try:
        collector = Collector(inc)
    except IOError:
        msg = 'ERROR: the path `%s` does contain the spp header ' \
              'files.\n' % os.path.abspath(inc)
        error(msg)

    collector['USER'] = os.environ['USER']
    collector['DATE'] = date()
    collector['METHODNAME'] = str(arg.method)
    if arg.model == '':
        collector['MODELNAME'] = '%s_model' % arg.type
    else:
        collector['MODELNAME'] = str(arg.model)
    collector['COLLECTORNAME'] = str(arg.collector)

    _write_method_props(collector, cfg['mkmethod'], inc)
    _write_model_props(arg, collector, cfg['mkmodel'], inc)

    print collector

    if arg.w is True:
        mdir = cfg['mkcollector']['collector_dir']
        if mdir[0] != '/':
            mdir = '%s/%s' % (root, mdir)
        fname = '%s.h' % (arg.collector)
        print 'Writing %s to %s ...' % (fname, mdir)
        absfname = '%s/%s' % (mdir, fname)
        if os.path.exists(absfname):
            error('ERROR: file already exist\n')
        if not os.path.exists(mdir):
            error('ERROR: directory does not exist\n')
        with open(absfname, 'w') as tmpf:
            tmpf.write(str(collector))
        print 'done ...'
