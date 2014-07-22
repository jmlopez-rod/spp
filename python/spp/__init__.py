"""Simulating Population Processes

This python package is to be used together with the c++ header files
to create simulations.

"""

import os
import sys
from subprocess import Popen, PIPE
from dateutil import parser
from datetime import datetime


def error(msg):
    "Print a message to the standard error stream and exit. "
    sys.stderr.write(msg)
    sys.exit(2)


def trace(msg):
    "Print a message to the standard error "
    sys.stderr.write(msg)


def exec_cmd(cmd, verbose=False):
    "Run a subprocess and return its output and errors. "
    if verbose:
        out = sys.stdout
        err = sys.stderr
    else:
        out = PIPE
        err = PIPE
    process = Popen(cmd, shell=True, stdout=out, stderr=err)
    out, err = process.communicate()
    return out, err, process.returncode


def date(short=False):
    "Return the current date as a string. "
    if isinstance(short, str):
        now = parser.parse(short)
        return now.strftime("%a %b %d, %Y %r")
    now = datetime.now()
    if not short:
        return now.strftime("%a %b %d, %Y %r")
    return now.strftime("%Y-%m-%d-%H-%M-%S")


def import_mod(name):
    "Return a module by string. "
    mod = __import__(name)
    for sub in name.split(".")[1:]:
        mod = getattr(mod, sub)
    return mod


class Template(object):
    """Basic structure to read and write templates. """
    def __init__(self, filename):
        with open(filename, 'r') as tfn:
            self.text = tfn.read()
            self.var = dict()

    def __setitem__(self, var, val):
        """x.__setitem__(var) = val <==> x[var] = val """
        self.var[var] = val

    def __getitem__(self, var):
        """x.__setitem__(var) <==> x[var] """
        return self.var[var]

    def __delitem__(self, var):
        """x.__delitem__(var) <==> del x[var] """
        del self.var[var]

    def __len__(self):
        return len(self.var)

    def __str__(self):
        """x.__str__() <==> str(x)"""
        tmp = self.text
        for key in self.var:
            tmp = tmp.replace('$(%s)' % key, self.var[key])
            tmp = tmp.replace('${UPPER}(%s)' % key, self.var[key].upper())
        return tmp

    def __repr__(self):
        """x.__repr__() <==> repr(x)"""
        tmp = 'maker.template[0x%x]:\n' % id(self)
        for key in self.var:
            tmp += '  %s: %s\n' % (key, repr(self.var[key]))
        tmp += 'Data: \n'
        tmp += self.text
        tmp += '\n-----'
        return tmp
